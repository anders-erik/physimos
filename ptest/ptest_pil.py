
from PIL import Image
import numpy as np

import sys

import ptest_util
import ptest_config

def pillow_open_image(dir_path):
    with Image.open(dir_path + "/2x2.bmp") as im:
        im.rotate(45).show()

def pillow_write_new_file_to_tmp(dir_path):
    # imarray = [0, 0, 0, 66, 66, 66, 150, 150, 150, 255, 255, 255]
    # im = Image.fromarray(imarray.astype('uint8'))
    a =   np.zeros((5, 5))
    a_2 = np.zeros(shape=(5,5), dtype=np.float64)
    a_255 = np.full([5, 5], 255.)
    # im = Image.fromarray(a_255).convert('RGB')
    # print(a)

    b = [[1, 2, 3], [4, 5, 6]]
    
    np_b = np.array(b)

    c = [[0., 0., 0., 66., 66., 66.], [150., 150., 150., 255., 255., 255.]]
    # np_c = np.array(c)
    # c2 = [[65535, 0], [100000, 0]]
    # c2 = [[255, 255], [255, 255]]
    c2 = [[555, 255, 255], [255, 255, 255]]

    c3 = np.array([
        [[255, 0, 0], [0, 255, 0]],  # Red, Green
        [[0, 0, 255], [255, 255, 0]] # Blue, Yellow
    ], dtype=np.uint8)
    im = Image.fromarray(c3)

    np_c = np.array(c3)

    print(np_c)
    print(np.zeros((2, 2)))
    # im = Image.fromarray(np_c, mode="RGB")
    print(im.getpixel((0, 0)))
    print(im.getpixel((0, 1)))
    print(im.getpixel((1, 0)))
    print(im.getpixel((1, 1)))

    np_rr = np.random.rand(100,100,3) * 255
    # im = Image.fromarray(np_rr.astype('uint8')).convert('RGBA')

    
    im.save(dir_path + '/new_image.BMP', 'BMP')
    # im.save('new_image.png', "PNG")


def generate_100x100_random_image(outDir):

    pimage_gen_paths = ptest_config.get_pimage_gen_paths_dict()

    
    igm_100x100_rand_file_name = pimage_gen_paths.get('100x100_rand')[0] # first entry is the gen path, [1] is the test output file
    igm_100x100_rand_file_absolute = outDir + "/" + igm_100x100_rand_file_name

    np_100x100_rand = np.random.rand(100,100,3) * 255
    igm_100x100_rand = Image.fromarray(np_100x100_rand.astype('uint8')).convert('RGBA')

    igm_100x100_rand.save(igm_100x100_rand_file_absolute, 'BMP')

    print("generate_test_file -> " + igm_100x100_rand_file_absolute)



def print_help():
    print("")
    print(" Pillow methods:")
    print("")
    print(" -g = generate_test_files(tmpDir) ")
    print("")

if(len(sys.argv) == 1): # no arguments = watch mode
    print_help()
elif(sys.argv[1] == '-h'):
    print_help()
elif(sys.argv[1] == '-g'):
    out_dir = ptest_util.get_physimos_root_dir() + "/tmp"
    generate_100x100_random_image(out_dir)
# else: 
#     print("")
#     print("Unknown Ptest CLI argument. Use '-h' for help. ")
#     print("")
