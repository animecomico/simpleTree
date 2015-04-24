/*
* Software License Agreement (BSD License)
*
* Copyright (c) 2015, Jan Hackenberg, University of Freiburg.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above
* copyright notice, this list of conditions and the following
* disclaimer in the documentation and/or other materials provided
* with the distribution.
* * Neither the name of Willow Garage, Inc. nor the names of its
* contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*/
#ifndef CURVATUREDIALOG_H
#define CURVATUREDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/common/centroid.h>
#include <pcl/common/common.h>
#include <pcl/point_types.h>

#include <Eigen/Eigenvalues>

#include "../../../build/ui_curvature_dialog.h"
#include "../../../src/gui/pclviewer.h"

#include "../../../src/gui/guisubclass.h"


typedef pcl::PointXYZRGBA PointD;
typedef pcl::PointCloud<PointD> PointCloudD;
typedef pcl::PointXYZINormal PointI;
typedef pcl::PointCloud<PointI> PointCloudI;

class CurvatureDialog : public QDialog, public GuiSubClass
{
    Q_OBJECT
public:
    explicit CurvatureDialog(QWidget *parent = 0);
    void
    setViewer(boost::shared_ptr<PCLViewer> guiPtr);
    boost::shared_ptr<PCLViewer>
    getViewer();
    void
    init();
private:

    std::vector<float> e1,e2,e3;
    float
    min_e1,max_e1,min_e2,max_e2,min_e3,max_e3;
    float
    min_pca1,max_pca1,min_pca2,max_pca2,min_pca3,max_pca3;
    boost::shared_ptr<Ui_Dialog_Eigen> dialog;
    boost::weak_ptr<PCLViewer> viewer;
    boost::shared_ptr<PointCloudD> visu_cloud;
    void
    resetViewer();
    void
    updateViewer();
    void
    setGreen(PointD & p);
    void
    setRed(PointD & p);

signals:

public slots:
    void
    minPC1();
    void
    maxPC1();
    void
    minPC2();
    void
    maxPC2();
    void
    minPC3();
    void
    maxPC3();
    void
    abort();
    void
    save();

};

#endif // CURVATUREDIALOG_H
