# censoring.py
from nudenet import NudeDetector
import sys
import sqlite3
from sqlite3 import Error
# When running the first time, it will download the default checkpoint
# e.g Downloading the checkpoint to C:\Users\sdkca\.NudeNet/default\detector_v2_default_checkpoint_tf
detector = NudeDetector()

# Analyze the image1.jpg and create a new one with the censored suffix
#detector.censor(
#    './pic.jpeg', 
#    out_path='./pic_censored.jpeg', 
#    visualize=False
#)


def censoring():
    #get file path
    source_file_path = sys.argv[1]
    contains_space = source_file_path.__contains__(" ")
    #print(contains_space)

    #get_censor_path
    dest_file_path = sys.argv[2]
    contains_spacee = dest_file_path.__contains__(" ")
    #print(contains_spacee)
    #print("censor_pthh--",dest_file_path)

    #output_path = dest_file_path + file_record_no + "_censored_" + input_file_name
    output_path = dest_file_path

    #print("out---",output_path)


    detector.censor(
        source_file_path,
        out_path=output_path, 
        visualize=False
    )
    #print("censor_done")


def main():
    censoring()


if __name__ == '__main__':
    main()
