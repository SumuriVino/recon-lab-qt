from PIL import Image, ImageFilter
import sys

if len(sys.argv) != 3:
    print("\nUsage: Please provide proper input \n")
    sys.exit(0)

file_path = sys.argv[1]
dest_file_path = sys.argv[2]

Original_img = Image.open(file_path)
if not Original_img:
    Original_img.save(dest_file_path)
else:
    gaussImage = Original_img.filter(ImageFilter.GaussianBlur(8))
    gaussImage.save(dest_file_path)




