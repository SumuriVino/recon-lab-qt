import cv2
import numpy as np
import time
import sys
import sqlite3
from sqlite3 import Error
import json
import itertools
import os

source_count_name = sys.argv[1]
os_scheme_internal = sys.argv[2]
virtual_source_path = sys.argv[3]
tmp_fs_db_path = sys.argv[4]
weapons_dp_path = sys.argv[5]
yolo_weigths_script_path = sys.argv[6]
yolo_cfg_script_path = sys.argv[7]
object_script_path = sys.argv[8]
pref_min_file_size = int(sys.argv[9])
bool_runner_type = sys.argv[10]
complete_path_list = sys.argv[11]
category_gun = sys.argv[12]
category_rifles = sys.argv[13]

def get_box_dimensions(outputs, height, width):
        boxes = []
        confs = []
        class_ids = []
        for output in outputs:
                for detect in output:
                        scores = detect[5:]
                        class_id = np.argmax(scores)
                        conf = scores[class_id]
                        if conf > 0.3:
                                center_x = int(detect[0] * width)
                                center_y = int(detect[1] * height)
                                w = int(detect[2] * width)
                                h = int(detect[3] * height)
                                x = int(center_x - w/2)
                                y = int(center_y - h / 2)
                                boxes.append([x, y, w, h])
                                confs.append(float(conf))
                                class_ids.append(class_id)
        return boxes, confs, class_ids

def detect_objects(img, net, outputLayers):
        blob = cv2.dnn.blobFromImage(img, scalefactor=0.00392, size=(320, 320), mean=(0, 0, 0), swapRB=True, crop=False)
        net.setInput(blob)
        outputs = net.forward(outputLayers)
        return blob, outputs

def load_image(img_path):
    img = cv2.imread(img_path)
    if img is None:
        img = 'no'
        height = 'no'
        width = 'no'
        channels = 'no'
        return img, height, width, channels
    img = cv2.resize(img, None, fx=1, fy=1)
    height, width, channels = img.shape
    return img, height, width, channels    

def load_yolo():
    net = cv2.dnn.readNet(yolo_weigths_script_path, yolo_cfg_script_path)
    classes = []
    with open(object_script_path, "r") as f:
            classes = [line.strip() for line in f.readlines()]
    layers_names = net.getLayerNames()
    output_layers = [layers_names[i-1] for i in net.getUnconnectedOutLayers()]
    colors = np.random.uniform(0, 255, size=(len(classes), 3))
    return net, classes, colors, output_layers


def image_detect(file_path):
    model, classes, colors, output_layers = load_yolo()
    image, height, width, channels = load_image(file_path)
    if image == 'no':
        return
    blob, outputs = detect_objects(image, model, output_layers)
    boxes, confs, class_ids = get_box_dimensions(outputs, height, width)
    
    if len(class_ids) == 0:
        return None
    check =  class_ids[0]
    if check == 1:
       return None
    return check


def full_source_extraction():
    conn = None
    conn = sqlite3.connect(tmp_fs_db_path)

    cur = conn.cursor()

    if (os_scheme_internal == "iOS_UID" or os_scheme_internal == "iOS_XML"):
        cur.execute("SELECT id_path , INT , file_size, mime_type FROM files WHERE fs_module_weapons_run_status='0' And file_size is Not NULL")
    else:
        cur.execute("SELECT filepath  , INT , file_size, mime_type  FROM files WHERE fs_module_weapons_run_status='0' And file_size is Not NULL")
        #cur.execute("SELECT filepath  , INT , file_size, mime_type  FROM files WHERE fs_module_guns_and_fire_detection_run_status='0' And file_size is Not NULL")

    rows = cur.fetchall()
    size = len(rows)
    for row in rows:
        file_size = int(row[2])
        if(file_size < pref_min_file_size):
           continue

        mime_type = row[3]
        if mime_type == None:
            continue

        if not(mime_type.startswith("image/")):
           continue       

        print("INT",row[1])
        sys.stdout.flush()
        if(row[0] == None):
           continue
        file_path = ''.join(row[0])
        file_int = json.dumps(row[1])

        full_file_path = virtual_source_path + file_path
        double_slash = full_file_path.__contains__('//')
        
        if double_slash is True:
            full_file_path = full_file_path.replace('//','/')

            output = image_detect(full_file_path)
            if output is None:
                continue
            if output == 0:
                out_type = category_gun
            if output == 2:
                out_type = category_rifles

            con = sqlite3.connect(weapons_dp_path)
            c = con.cursor()
            c.execute("insert into files (filesystem_record,weapon_type) values (?,?)", (row[1],out_type))
            con.commit();

        cv2.destroyAllWindows()
    

def right_click_extraction():
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
                cur.execute("SELECT id_path , INT , file_size, mime_type FROM files WHERE fs_module_weapons_run_status='0' And file_size is Not NULL And filepath = '" + file_path + "'")
            else:
                if not(file_path.endswith("/")):
                    file_path = file_path + "/"
                    
                cur.execute("SELECT id_path , INT , file_size, mime_type FROM files WHERE fs_module_weapons_run_status='0' And file_size is Not NULL And filepath LIKE '" + file_path + "%'")
        else:
            if(is_dir == 0):
                cur.execute("SELECT filepath  , INT , file_size, mime_type  FROM files WHERE fs_module_weapons_run_status='0' And file_size is Not NULL And filepath = '" + file_path + "'")
            else:
                if not(file_path.endswith("/")):
                    file_path = file_path + "/"
        
                cur.execute("SELECT filepath  , INT , file_size, mime_type  FROM files WHERE fs_module_weapons_run_status='0' And file_size is Not NULL And filepath LIKE '" + file_path + "%'")
            #cur.execute("SELECT filepath  , INT , file_size, mime_type  FROM files WHERE fs_module_guns_and_fire_detection_run_status='0' And file_size is Not NULL And filepath LIKE '" + file_path + "%'")

        rows = cur.fetchall()
        size = len(rows)

        for row in rows:
            print("INT",row[1])
            sys.stdout.flush()
        
            f_size = row[2]
        
            if(f_size < pref_min_file_size):
                continue

            mime_type = row[3]
            if mime_type == None:
                continue
                
            if not(mime_type.startswith("image/")):
                continue
                
            #print("INT",row[1])
            #sys.stdout.flush()
            
            file_path = ''.join(row[0])

            full_file_path = virtual_source_path + file_path
            double_slash = full_file_path.__contains__('//')

            if double_slash is True:
                full_file_path = full_file_path.replace('//','/')

            output = image_detect(full_file_path)
            if output is None:
                continue
            if output == 0:
                out_type = "Gun"
            if output == 2:
                out_type = "Rifles"

            con = sqlite3.connect(weapons_dp_path)
            c = con.cursor()
            c.execute("insert into files (filesystem_record,weapon_type) values (?,?)", (row[1],out_type))
            con.commit();

        cv2.destroyAllWindows()

def create_connection(tmp_fs_db_path):
    conn = None
    try:
        conn = sqlite3.connect(tmp_fs_db_path)
        
    except Error as e:
        print(e)
    return conn

def main():

    if bool_runner_type == 'TRUE':
        right_click_extraction()
    else:
        full_source_extraction()
        #conn = create_connection(tmp_fs_db_path)
        #with conn:
        #select_all_paths(conn)

if __name__ == '__main__':
    main()


    
