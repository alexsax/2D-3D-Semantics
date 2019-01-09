/*
 *   pcl_utils.cpp - Some convenience functions for using data from
 *       Joint 2D-3D-Semantic Data for Indoor Scene Understanding
 *       in conjunction with Point Cloud Library (PCL)
 *
 *      Website: 3dsemantics.stanford.edu
 *      Paper: https://arxiv.org/pdf/1702.01105.pdf 
 *  
 *  Usage: Copy or include the code
 */
#ifndef SEMANTIC2D3D_UTILS_H
#define SEMANTIC2D3D_UTILS_H

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <pcl/common/eigen.h>

namespace utils
{

    /** Gets the camera translation from a pose file ptree and returns it**/
    inline Eigen::Affine3f
    getCameraTranslation( const boost::property_tree::ptree &pt ){
        // Read in camera translation
        float tr[3];
        int i = 0;
        boost::property_tree::ptree::const_iterator end = pt.get_child("camera_location").end();
        for (boost::property_tree::ptree::const_iterator it = pt.get_child("camera_location").begin(); it != end; ++it, ++i) {
            tr[i] = it->second.get_value<float>();
        }
        Eigen::Affine3f translation(Eigen::Translation3f(tr[0], tr[1], tr[2]));
        return translation;
    }

    /** Gets the camera rotation from a pose file ptree and returns it**/
    inline Eigen::Affine3f
    getCameraRotation( const boost::property_tree::ptree &pt ){
        // Read in the camera euler angles
        int i = 0;
        float euler_angles[3];
        boost::property_tree::ptree::const_iterator end = pt.get_child("camera_rotation_final").end();
        for (boost::property_tree::ptree::const_iterator it = pt.get_child("camera_rotation_final").begin(); it != end; ++it, ++i) {
            euler_angles[i] = it->second.get_value<float>();
            std::cout << "Angle " << i << ": " << euler_angles[i] << std::endl;
        }
        //Roll pitch and yaw in Radians
        float roll = euler_angles[2], pitch = euler_angles[1], yaw = euler_angles[0];
        std::cout << roll << ", " << pitch <<  ", " << yaw << std::endl;
        Eigen::Quaternionf q;
        q = Eigen::AngleAxisf(roll, Eigen::Vector3f::UnitZ())
            * Eigen::AngleAxisf(pitch, Eigen::Vector3f::UnitY())
            * Eigen::AngleAxisf(yaw, Eigen::Vector3f::UnitX());
        Eigen::Affine3f rotation( q );
        return rotation;
    }

    /** Load in the json pose files into a boost::ptree **/
    inline boost::property_tree::ptree 
    loadPoseFile( const std::string &json_filename ) {
        // Read in view_dict
        boost::property_tree::ptree pt;
        std::ifstream in(json_filename);
        std::stringstream buffer;
        buffer << in.rdbuf();    
        read_json(buffer, pt);
        return pt;
    }
    
    /** Debugging function to print out a boost::ptree **/
    void print(boost::property_tree::ptree const& pt){
    using boost::property_tree::ptree;
    ptree::const_iterator end = pt.end();
    for (ptree::const_iterator it = pt.begin(); it != end; ++it) {
        std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
        print(it->second);
    }
}
} // END namespace

#endif  // #ifndef SEMANTIC2D3D_UTILS_H
