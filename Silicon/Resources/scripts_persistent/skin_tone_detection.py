# classification.py

# Import module
from nudenet import NudeClassifier
import sqlite3
from sqlite3 import Error
#import magic
import mimetypes
import filetype
import os
#import pandas as pd
import json
import sys
#import moviepy.editor
import glob
import itertools
#from decimal import Decimal
# initialize classifier (downloads the checkpoint file automatically the first time)
classifier = NudeClassifier()

# A. Classify single image
source_count_name = sys.argv[1]
os_scheme_internal = sys.argv[2]
virtual_source_path = sys.argv[3]
tmp_fs_db_path = sys.argv[4]
skin_tone_detection_db_path = sys.argv[5]
video_thumb_dir_path = sys.argv[6]
min_file_size = int(sys.argv[7])
bool_runner_type = sys.argv[8]

complete_path_list = sys.argv[9]

def create_connection(db_file):

    conn = None
    try:
        conn = sqlite3.connect(db_file)

    except Error as e:
        print(e)
    return conn


def select_all_tasks(conn):
    cur = conn.cursor()

    if (os_scheme_internal == "iOS_UID" or os_scheme_internal == "iOS_XML"):
        cur.execute("SELECT id_path,mime_type,INT FROM files WHERE fs_module_skin_tone_detection_run_status='0' AND mime_type NOT NULL")
    else:
        cur.execute("SELECT filepath,mime_type,INT FROM files WHERE fs_module_skin_tone_detection_run_status = 0 AND mime_type NOT NULL")
    
    rows = cur.fetchall()
    si = len(rows)

    for row in rows:
        print("INT",row[2])
        sys.stdout.flush()
        str = ''.join(row[0])
        file_int = json.dumps(row[2])
        
        str = virtual_source_path + str
       
        double_slash = str.__contains__('//')
        if double_slash is True:
            str= str.replace('//','/')
        
        plist_file = str.__contains__('.plist')
        if plist_file is True:
            continue

        file_size = os.path.getsize(str)
        if (file_size < min_file_size):
            continue

        mime = row[1]  
        
        conn = sqlite3.connect(skin_tone_detection_db_path)
        cur = conn.cursor()


#check_for_image        
        image_file = mime.__contains__('image/')
#check_for_video        
        video_file = mime.__contains__('video/')
    #not_an_image
        not_image = mime.__contains__('application/x-matroska')
        if not_image is True:
            mime = get_mime_type(str)
            if mime.__contains__('video'):
                video_file = True
            if mime.__contains__('video'):
                video_file = True
            else:
                not_image = True

#VIDEO_WORK_START
        if video_file is True:
            cmd = "ffmpeg -i "+'"' +str+'"'+" -vf fps=1/4 " +'"'+video_thumb_dir_path+ "thumb%d.jpeg"+'"'
            os.system(cmd)
            for filename in os.listdir(video_thumb_dir_path):
                thumbnails = os.path.join(video_thumb_dir_path,filename)
                bool_unsafe = False
                nudes = classifier.classify([thumbnails])
                for key,value in nudes.items():
                    value = json.dumps(value) 
                
                    val1 = value.split(",")[0]
                    val2 = value.split(",")[1]

                    check = val2.__contains__('unsafe')
                    if check is True:
                        unsafe_val = val1.split(":")[1]                   
                        cur.execute("INSERT INTO files (filesystem_record,mime) VALUES (?,?)",(row[2],mime))
                        conn.commit()
                        os.remove(thumbnails)
                        bool_unsafe = True
                        break
                    else:
                        os.remove(thumbnails)
                if bool_unsafe is True:
                    break      
        #clear_dir
            files = glob.glob(video_thumb_dir_path + "*")
            for f in files:
                os.remove(f)
#VIDEO_WORK_END


#insert
        if mime is None:
            continue
        elif not_image is True:
            continue
        elif image_file is True:
            nudes = classifier.classify([str])
            for key,value in nudes.items():
                value = json.dumps(value) 
                
                val1 = value.split(",")[0]
                val2 = value.split(",")[1]

                check = val2.__contains__('unsafe')
                if check is True:
                    unsafe_val = val1.split(":")[1]                   
                    cur.execute("INSERT INTO files (filesystem_record,mime) VALUES (?,?)",(row[2],mime))
                    conn.commit()
        else:
           continue

def set_all_paths():
    
    conn = None
    con = sqlite3.connect(tmp_fs_db_path)

    cur = con.cursor()

    comp_pth_list = complete_path_list.split(",")

    for file_path in comp_pth_list:
        cur.execute("SELECT is_dir from files where filepath ='" + file_path + "' And is_dir is not NULL")
        roow = cur.fetchall()
        is_dir = None

        for ro in roow:
            is_dir = int(ro[0])
        
        if (os_scheme_internal == "iOS_UID" or os_scheme_internal == "iOS_XML"):
            if(is_dir == 0):
                cur.execute("SELECT id_path, mime_type, INT FROM files WHERE fs_module_skin_tone_detection_run_status='0' And file_size is Not NULL And mime_type NOT NULL And filepath = '" + file_path + "'")
            else:
                if not(file_path.endswith("/")):
                    file_path = file_path + "/"
                    
                cur.execute("SELECT id_path, mime_type, INT FROM files WHERE fs_module_skin_tone_detection_run_status='0' And file_size is Not NULL And mime_type NOT NULL And filepath LIKE '" + file_path + "%'")
        else:
            if(is_dir == 0):
                cur.execute("SELECT filepath, mime_type, INT  FROM files WHERE fs_module_skin_tone_detection_run_status='0' And file_size is Not NULL And mime_type NOT NULL And filepath = '" + file_path + "'")

            else:
                if not(file_path.endswith("/")):
                    file_path = file_path + "/"
        
                cur.execute("SELECT filepath, mime_type, INT FROM files WHERE fs_module_skin_tone_detection_run_status='0' And file_size is Not NULL And mime_type NOT NULL And filepath LIKE '" + file_path + "%'")

        
    rows = cur.fetchall()
    si = len(rows)

    for row in rows:
        print("INT",row[2])
        sys.stdout.flush()
        str = ''.join(row[0])
        file_int = json.dumps(row[2])
        
        str = virtual_source_path + str
       
        double_slash = str.__contains__('//')
        if double_slash is True:
            str= str.replace('//','/')
        
        plist_file = str.__contains__('.plist')
        if plist_file is True:
            continue

        file_size = os.path.getsize(str)
        if (file_size < min_file_size):
            continue

        mime = row[1]
        
        conn = sqlite3.connect(skin_tone_detection_db_path)
        cur = conn.cursor()

        if mime == None:
            continue

#check_for_image
        image_file = mime.__contains__('image/')
#check_for_video
        video_file = mime.__contains__('video/')
    #not_an_image
        not_image = mime.__contains__('application/x-matroska')
        if not_image is True:
            mime = get_mime_type(str)
            if mime.__contains__('video'):
                video_file = True
            if mime.__contains__('video'):
                video_file = True
            else:
                not_image = True

#VIDEO_WORK_START
        if video_file is True:
            cmd = "ffmpeg -i "+'"' +str+'"'+" -vf fps=1/4 " +'"'+video_thumb_dir_path+ "thumb%d.jpeg"+'"'
            os.system(cmd)
            for filename in os.listdir(video_thumb_dir_path):
                thumbnails = os.path.join(video_thumb_dir_path,filename)
                bool_unsafe = False
                nudes = classifier.classify([thumbnails])
                for key,value in nudes.items():
                    value = json.dumps(value)
                
                    val1 = value.split(",")[0]
                    val2 = value.split(",")[1]

                    check = val2.__contains__('unsafe')
                    if check is True:
                        unsafe_val = val1.split(":")[1]
                        cur.execute("INSERT INTO files (filesystem_record,mime) VALUES (?,?)",(row[2],mime))
                        conn.commit()
                        os.remove(thumbnails)
                        bool_unsafe = True
                        break
                    else:
                        os.remove(thumbnails)
                if bool_unsafe is True:
                    break
        #clear_dir
            files = glob.glob(video_thumb_dir_path + "*")
            for f in files:
                os.remove(f)
#VIDEO_WORK_END


#insert
        if mime is None:
            continue
        elif not_image is True:
            continue
        elif image_file is True:
            nudes = classifier.classify([str])
            for key,value in nudes.items():
                value = json.dumps(value)
                
                val1 = value.split(",")[0]
                val2 = value.split(",")[1]

                check = val2.__contains__('unsafe')
                if check is True:
                    unsafe_val = val1.split(":")[1]
                    cur.execute("INSERT INTO files (filesystem_record,mime) VALUES (?,?)",(row[2],mime))
                    conn.commit()
        else:
           continue

def get_mime(str):
    mime = mimetypes.MimeTypes().guess_type(str)[0]
    if mime is None:
        file_name, ext = os.path.splitext(str)
        check =  ext.__contains__('webp')
        if check is True:
            mime = 'image/webp' 
    return mime

def get_mime_type(str):
    mime_type = filetype.guess(str)
    return mime_type.mime



def main():

    if bool_runner_type == 'TRUE':
        set_all_paths()
    else:
        conn = create_connection(tmp_fs_db_path)
        with conn:
            select_all_tasks(conn)


if __name__ == '__main__':
    main()

# This would print something like:
# {
#   './image1.jpg': {
#      'safe': 0.00015856953, 
#      'unsafe': 0.99984145
#   }
# }

# B. Classify multiple images
# Returns {'path_to_image_1': {'safe': PROBABILITY, 'unsafe': PROBABILITY}}
# Classify multiple images (batch prediction)
# batch_size is optional; defaults to 4

#print(
 #   classifier.classify(
  #      ['./image1.jpg', './image2.jpg', './image3.jpg', './image4.jpg'],
   #     batch_size=4
    #)
#)

# {
#    './image1.jpg': {
#         'safe': 0.00015856922, 
#         'unsafe': 0.99984145
#     },
#     './image2.jpg': {
#         'safe': 0.019551795, 
#         'unsafe': 0.9804482
#     },
#    './image3.jpg': {
#         'safe': 0.00052562816,
#         'unsafe': 0.99947435
#    }, 
#    './image4.jpg': {
#         'safe': 3.3454136e-05,
#         'unsafe': 0.9999665
