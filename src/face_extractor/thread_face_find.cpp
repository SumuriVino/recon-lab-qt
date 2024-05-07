#include "thread_face_find.h"
#include <time.h>

#include <dlib/dnn.h>
#include <dlib/gui_widgets.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <QtCore>

using namespace dlib;
using namespace std;

void SaveCropImage(matrix<rgb_pixel>& matCrop, int nCropIndex);

// ----------------------------------------------------------------------------------------

// The next bit of code defines a ResNet network.  It's basically copied
// and pasted from the dnn_imagenet_ex.cpp example, except we replaced the loss
// layer with loss_metric and made the network somewhat smaller.  Go read the introductory
// dlib DNN examples to learn what all this stuff means.
//
// Also, the dnn_metric_learning_on_images_ex.cpp example shows how to train this network.
// The dlib_face_recognition_resnet_model_v1 model used by this example was trained using
// essentially the code shown in dnn_metric_learning_on_images_ex.cpp except the
// mini-batches were made larger (35x15 instead of 5x5), the iterations without progress
// was set to 10000, and the training dataset consisted of about 3 million images instead of
// 55.  Also, the input layer was locked to images of size 150.
template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128, avg_pool_everything<
    alevel0<
    alevel1<
    alevel2<
    alevel3<
    alevel4<
    max_pool<3, 3, 2, 2, relu<affine<con<32, 7, 7, 2, 2,
    input_rgb_image_sized<150>
    >>>>>>>>>>>>;

std::vector<matrix<rgb_pixel>> jitter_image(const matrix<rgb_pixel>& img)
{
    // All this function does is make 100 copies of img, all slightly jittered by being
    // zoomed, rotated, and translated a little bit differently. They are also randomly
    // mirrored left to right.
    thread_local dlib::rand rnd;

    std::vector<matrix<rgb_pixel>> crops;
    for (int i = 0; i < 100; ++i)
        crops.push_back(jitter_image(img, rnd));

    return crops;
}

thread_face_find::thread_face_find()
{
    m_bThreadExit = false;
}

//#define TEST_CROP_OUTLINE
#ifdef TEST_CROP_OUTLINE
void thread_face_find::run()
{
    int nCropIndex;
    string strPath;
    std::vector<string> vStrTargetFileList;

    for (const auto & entry : fs::directory_iterator(m_strTargetFolderPath.toStdString()))
    {
        strPath = entry.path().u8string();
        vStrTargetFileList.push_back(strPath);
    }

    // The first thing we are going to do is load all our models.  First, since we need to
    // find faces in the image we will need a face detector:
    frontal_face_detector detector = get_frontal_face_detector();

    // We will also use a face landmarking model to align faces to a standard pose:  (see face_landmark_detection_ex.cpp for an introduction)
    shape_predictor sp;
    deserialize("Models/dlib/shape_predictor_5_face_landmarks.dat") >> sp;

    // And finally we load the DNN responsible for face recognition.
    anet_type net;
    deserialize("Models/dlib/dlib_face_recognition_resnet_model_v1.dat") >> net;

    matrix<rgb_pixel> imgSrc;
    load_image(imgSrc, m_strSrcFilePath.toStdString());
    //image_window winTarget(imgSrc);

    std::vector<matrix<rgb_pixel>> facesSrc;
    for (auto face : detector(imgSrc))
    {
        auto shape = sp(imgSrc, face);
        matrix<rgb_pixel> face_chip;
        extract_image_chip(imgSrc, get_face_chip_details(shape, 150, 0.25), face_chip);
        facesSrc.push_back(face_chip);
        // Also put some boxes on the faces so we can see that the detector is finding them.
        //winTarget.add_overlay(face);
    }
    if (facesSrc.size() == 0)
    {
        emit ProgressChanged(-1);
        recon_static_functions::app_debug("No faces found in source image!",Q_FUNC_INFO);
        return;
    }

    // This call asks the DNN to convert each face image in faces into a 128D vector.
    // In this 128D vector space, images from the same person will be close to each other
    // but vectors from different people will be far apart.  So we can use these vectors to
    // identify if a pair of images are from the same person or from different people.
    std::vector<matrix<float, 0, 1>> descriptorsSrc = net(facesSrc);

    nCropIndex = 0;
    matrix<rgb_pixel> imgTarget;
    for (uint k = 0; k < vStrTargetFileList.size(); k++)
    {
        if( m_bThreadExit )
            break;

        load_image(imgTarget, vStrTargetFileList[k]);
        // Display the raw image on the screen
        image_window win(imgTarget);

        // Run the face detector on the image of our action heroes, and for each face extract a
        // copy that has been normalized to 150x150 pixels in size and appropriately rotated
        // and centered.
        std::vector<matrix<rgb_pixel>> facesTarget;
        for (auto face : detector(imgTarget))
        {
            auto shape = sp(imgTarget, face);
            matrix<rgb_pixel> face_chip;
            extract_image_chip(imgTarget, get_face_chip_details(shape, 150, 0.25), face_chip);

            time_t now;
            struct tm *tm;
            now = time(nullptr);
            tm = localtime (&now);
            string strTime = to_string(tm->tm_year+1900) +
                             to_string(tm->tm_mon+1) +
                             to_string(tm->tm_mday) +
                             to_string(tm->tm_hour) +
                             to_string(tm->tm_min) +
                             to_string(tm->tm_sec) +
                             to_string(nCropIndex);
            string strCroppedImage = "CropImages/" + strTime + ".jpg";
            save_jpeg(face_chip, strCroppedImage);
            nCropIndex++;

            facesTarget.push_back(face_chip);
            // Also put some boxes on the faces so we can see that the detector is finding them.
            win.add_overlay(face);
        }

        if (facesTarget.size() == 0)
        {
            cout << "No faces found in image!" << endl;
            continue;
        }

        // This call asks the DNN to convert each face image in faces into a 128D vector.
        // In this 128D vector space, images from the same person will be close to each other
        // but vectors from different people will be far apart.  So we can use these vectors to
        // identify if a pair of images are from the same person or from different people.
        std::vector<matrix<float, 0, 1>> descriptorsTarget = net(facesTarget);

        // In particular, one simple thing we can do is face clustering.  This next bit of code
        // creates a graph of connected faces and then uses the Chinese whispers graph clustering
        // algorithm to identify how many people there are and which faces belong to whom.
        std::vector<sample_pair> edges;

        if( m_fThreshold == 0.0f )
            m_fThreshold = 0.6f;

        for (size_t i = 0; i < descriptorsTarget.size(); ++i)
        {
            // Faces are connected in the graph if they are close enough.  Here we check if
            // the distance between two face descriptors is less than 0.6, which is the
            // decision threshold the network was trained to use.  Although you can
            // certainly use any other threshold you find useful.

            if (length(descriptorsTarget[i] - descriptorsSrc[0]) < m_fThreshold )
            {
                emit FaceFound(QString::fromStdString(vStrTargetFileList[k]));
            }
            //	edges.push_back(sample_pair(i, j));
        }

        std::vector<unsigned long> labels;
        const auto num_clusters = chinese_whispers(edges, labels);
        // This will correctly indicate that there are 4 people in the image.

        string strCount = std::to_string(num_clusters);
        recon_static_functions::app_debug("number of people found in the image: " + strCount.c_str(),Q_FUNC_INFO);

        int nIndex = k + 1;
        emit ProgressChanged(nIndex);
    }

    emit ProgressChanged(0);
    recon_static_functions::app_debug("completed",Q_FUNC_INFO);
}
#else
void thread_face_find::run()
{ // start thread and face search starts from here
    bool_cancel_extraction = false;

    emit signal_PBAR_thread_file_system_started(MACRO_JobType_Face_Search);

    bool bCropSaved = false;
    int nRepeat;
    string strPath;
    std::vector<string> vStrTargetFileList;
    matrix<rgb_pixel> matCrop;

    emit signal_PBAR_thread_file_system_value(MACRO_JobType_Face_Search, "Search Result", true, MACRO_Generic_Progress_No_Records_int, -1, true);

    for(int i = 0; i<target_path_QStringList.size(); i++)
    {
        if(bool_cancel_extraction)
            break;

        QString path = target_path_QStringList.at(i);

        strPath = QFileInfo(path).filePath().toStdString();

        vStrTargetFileList.push_back(strPath);
    }


    // The first thing we are going to do is load all our models.  First, since we need to
    // find faces in the image we will need a face detector:
    frontal_face_detector face_detector = get_frontal_face_detector();

    // We will also use a face landmarking model to align faces to a standard pose:  (see face_landmark_detection_ex.cpp for an introduction)
    shape_predictor shape_predictor_obj;
    deserialize("../Resources/features_resources/face_analysis/Models/dlib/shape_predictor_68_face_landmarks.dat") >> shape_predictor_obj;

    // And finally we load the DNN responsible for face recognition.
    anet_type net;
    deserialize("../Resources/features_resources/face_analysis/Models/dlib/dlib_face_recognition_resnet_model_v1.dat") >> net;


    matrix<rgb_pixel> imgSrc;
    load_image(imgSrc, m_strSrcFilePath.toStdString());

    std::vector<matrix<rgb_pixel>> facesSrc;
    for (auto face : face_detector(imgSrc))
    {
        if(bool_cancel_extraction)
            break;

        auto shape = shape_predictor_obj(imgSrc, face);
        matrix<rgb_pixel> face_chip;
        extract_image_chip(imgSrc, get_face_chip_details(shape, 150, 0.25), face_chip);
        facesSrc.push_back(face_chip);
        // Also put some boxes on the faces so we can see that the detector is finding them.
        //winTarget.add_overlay(face);
    }

    if (facesSrc.size() == 0)
    {
        emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Face_Search);
        emit signal_finished();
        return;
    }

    // This call asks the DNN to convert each face image in faces into a 128D vector.
    // In this 128D vector space, images from the same person will be close to each other
    // but vectors from different people will be far apart.  So we can use these vectors to
    // identify if a pair of images are from the same person or from different people.
    std::vector<matrix<float, 0, 1>> descriptorsSrc = net(facesSrc);

    nRepeat = 0;
    matrix<rgb_pixel> imgTemp, imgTarget;

    for (uint k = 0; k < vStrTargetFileList.size(); k++)
    {

        if(bool_cancel_extraction)
            break;

        if( m_bThreadExit )
            break;

        load_image(imgTarget, vStrTargetFileList[k]);

        std::vector<matrix<rgb_pixel>> facesTarget;
        std::vector<matrix<rgb_pixel>> vCropImage;

        nRepeat = 0;
        while( nRepeat < 3 )
        {
            if(bool_cancel_extraction)
                break;

            if( nRepeat == 1 )
                resize_image(2.0, imgTarget);
            else if( nRepeat == 2 )
                resize_image(4.0, imgTarget);

            // Display the raw image on the screen
            //image_window win(imgTarget);

            // Run the face detector on the image of our action heroes, and for each face extract a
            // copy that has been normalized to 150x150 pixels in size and appropriately rotated
            // and centered.

            for (auto face : face_detector(imgTarget))
            {
                if(bool_cancel_extraction)
                    break;

                auto shape = shape_predictor_obj(imgTarget, face);
                matrix<rgb_pixel> face_chip;
                extract_image_chip(imgTarget, get_face_chip_details(shape, 150, 0.25), face_chip);
                facesTarget.push_back(face_chip);

                matCrop = face_chip;
                set_image_size(matCrop, 150, 150);

                if( nRepeat == 1 )
                    resize_image(0.5, matCrop);
                else if( nRepeat == 2 )
                    resize_image(0.25, matCrop);

                vCropImage.push_back(matCrop);

                // Also put some boxes on the faces so we can see that the detector is finding them.
            }

            if (facesTarget.size() > 0)
                break;

            nRepeat++;
        }

        if( nRepeat == 3 )
            continue;

        // This call asks the DNN to convert each face image in faces into a 128D vector.
        // In this 128D vector space, images from the same person will be close to each other
        // but vectors from different people will be far apart.  So we can use these vectors to
        // identify if a pair of images are from the same person or from different people.
        std::vector<matrix<float, 0, 1>> descriptorsTarget = net(facesTarget);

        // In particular, one simple thing we can do is face clustering.  This next bit of code
        // creates a graph of connected faces and then uses the Chinese whispers graph clustering
        // algorithm to identify how many people there are and which faces belong to whom.
        std::vector<sample_pair> edges;

        if( m_fThreshold == 0.0f )
            m_fThreshold = 0.5f;

        bool bool_face_found_status = false;
        for (size_t i = 0; i < descriptorsTarget.size(); ++i)
        {
            if(bool_cancel_extraction)
                break;
            // Faces are connected in the graph if they are close enough.  Here we check if
            // the distance between two face descriptors is less than 0.6, which is the
            // decision threshold the network was trained to use.  Although you can
            // certainly use any other threshold you find useful.

            if (length(descriptorsTarget[i] - descriptorsSrc[0]) < m_fThreshold )
            {

                emit FaceFound(QString::fromStdString(vStrTargetFileList[k]));

                bool_face_found_status = true;
            }

        }

        if(bool_face_found_status)
        {
            break;
        }

    }

    emit signal_add_all_matches_to_case_tree();

    emit signal_PBAR_thread_file_system_finished(MACRO_JobType_Face_Search);

    emit signal_finished();
}


#endif

void thread_face_find::pub_set_bool_cancel_extraction(bool status)
{    
    bool_cancel_extraction = status;
}

void SaveCropImage(matrix<rgb_pixel>& matCrop, int nCropIndex)
{
    time_t now;
    struct tm *tm;
    now = time(nullptr);
    tm = localtime (&now);
    string strTime = to_string(tm->tm_year+1900) +
                     to_string(tm->tm_mon+1) +
                     to_string(tm->tm_mday) +
                     to_string(tm->tm_hour) +
                     to_string(tm->tm_min) +
                     to_string(tm->tm_sec) +
                     to_string(nCropIndex);
    string strCroppedImage = "CropImages/" + strTime + ".jpg";
    save_jpeg(matCrop, strCroppedImage);
}
