// Author: Benjamin Hilton
// Date: November 2018

#include <gtest/gtest.h>
#include "osgwidget.h"
#include "OSGLarynx.h"
#include <osg/Node>

TEST(STLFiles, requestToLoadThyroid_LoadsCorrectly)
{
    unsigned int numberRepresentingThyroid{1};
    EXPECT_NE(create_cartilage(numberRepresentingThyroid), nullptr);
}


TEST(STLFiles, requestToLoadCricoid_LoadsCorrectly)
{
    unsigned int numberRepresentingCricoid{2};
    EXPECT_NE(create_cartilage(numberRepresentingCricoid), nullptr);
}


TEST(STLFiles, requestToLoadArytenoid_LoadsCorrectly)
{
    unsigned int numberRepresentingArytenoid{3};
    EXPECT_NE(create_cartilage(numberRepresentingArytenoid), nullptr);
}
