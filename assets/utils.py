"""
    utils.py - Some convenience functions for using data from
        Joint 2D-3D-Semantic Data for Indoor Scene Understanding
           I. Armeni*, A. Sax*, A. Zamir, S. Savarese
        Website: 3dsemantics.stanford.edu
        Paper: https://arxiv.org/pdf/1702.01105.pdf 
        
    Code Author: Alexander Sax
    
    Usage: For import only. i.e. 'import utils.py'
      Dependencies include scipy, OpenEXR
"""

import array
import Imath
import json
import numpy
import OpenEXR
from   scipy.ndimage import imread

""" Semantics """
def get_index( color ):
    ''' Parse a color as a base-256 number and returns the index
    Args:
        color: A 3-tuple in RGB-order where each element \in [0, 255]
    Returns:
        index: an int containing the indec specified in 'color'
    '''
    return color[0] * 256 * 256 + color[1] * 256 + color[2]

def get_color( i ):
    ''' Parse a 24-bit integer as a RGB color. I.e. Convert to base 256
    Args:
        index: An int. The first 24 bits will be interpreted as a color.
            Negative values will not work properly.
    Returns:
        color: A color s.t. get_index( get_color( i ) ) = i
    '''
    b = ( i ) % 256  # least significant byte
    g = ( i >> 8 ) % 256
    r = ( i >> 16 ) % 256 # most significant byte 
    return r,g,b

""" Label functions """
def load_labels( label_file ):
    """ Convenience function for loading JSON labels """
    with open( label_file ) as f:
        return json.load( f )

def parse_label( label ):
    """ Parses a label into a dict """
    res = {}
    clazz, instance_num, room_type, room_num, area_num = label.split( "_" )
    res[ 'instance_class' ] = clazz
    res[ 'instance_num' ] = int( instance_num )
    res[ 'room_type' ] = room_type
    res[ 'room_num' ] = int( room_num )
    res[ 'area_num' ] = int( area_num )
    return res


""" EXR Functions """
def normalize_array_for_matplotlib( arr_to_rescale ):
    ''' Rescales an array to be between [0, 1]
    Args:
        arr_to_rescale:
    Returns:
        An array in [0,1] with f(0) = 0.5
    '''
    return ( arr_to_rescale / np.abs( arr_to_rescale ).max() ) / 2 + 0.5

def read_exr( image_fpath ):
    """ Reads an openEXR file into an RGB matrix with floats """
    f = OpenEXR.InputFile( image_fpath )
    dw = f.header()['dataWindow']
    w, h = (dw.max.x - dw.min.x + 1, dw.max.y - dw.min.y + 1)    
    im = np.empty( (h, w, 3) )

    # Read in the EXR
    FLOAT = Imath.PixelType(Imath.PixelType.FLOAT)
    channels = f.channels( ["R", "G", "B"], FLOAT )
    for i, channel in enumerate( channels ):
        im[:,:,i] = np.reshape( array.array( 'f', channel ), (h, w) )
    return im


