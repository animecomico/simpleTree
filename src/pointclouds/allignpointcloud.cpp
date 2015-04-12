#include "allignpointcloud.h"

AllignPointCloud::AllignPointCloud()
{

}


void
AllignPointCloud::getRootPoint(boost::shared_ptr<PointCloudI> cloud,Eigen::Vector4f & base, float minHeight)
{
    PointI p1;
    PointI p2;
    pcl::getMinMax3D<PointI>(*cloud,p1,p2);
    minHeight = p1.z;
    boost::shared_ptr<PointCloudI> z_slice (new PointCloudI);
    extractZSlice(cloud, z_slice, minHeight);

    boost::shared_ptr<PointCloudI> cluster (new PointCloudI);
    extractLargestCluster(z_slice,cluster);

    pcl::compute3DCentroid(*cluster, base);
}

void
AllignPointCloud::initialAllign()
{
    float minHeight_source = std::numeric_limits<float>::max();
    Eigen::Vector4f root_source;
    getRootPoint(cloud_source,root_source,minHeight_source);
    transform<PointI>(cloud_source,root_source);

    float minHeight_target = std::numeric_limits<float>::max();
    Eigen::Vector4f root_target;
    getRootPoint(cloud_target,root_target,minHeight_target);
    transform<PointI>(cloud_target,root_target);
}


void
AllignPointCloud::extractLargestCluster(boost::shared_ptr<PointCloudI> cloud_in, boost::shared_ptr<PointCloudI>  cloud_out, float distance)
{
    std::vector<pcl::PointIndices> cluster_indices;
    cloud_out.reset(new PointCloudI);

    pcl::search::KdTree<PointI>::Ptr tree ( new pcl::search::KdTree<PointI> );
    tree->setInputCloud ( cloud_in );

    pcl::EuclideanClusterExtraction<PointI> ec;
    ec.setClusterTolerance ( distance );
    ec.setMinClusterSize ( 1 );
    ec.setSearchMethod ( tree );
    ec.setInputCloud ( cloud_in );
    ec.extract ( cluster_indices );

    if ( cluster_indices.size () > 0 ) {
        pcl::PointIndices largestCluster = cluster_indices.at ( 0 );
        for ( std::vector<int>::const_iterator pit = largestCluster.indices.begin (); pit != largestCluster.indices.end (); pit++ )
            cloud_out->points.push_back ( cloud_in->points[*pit] );
    }

}


void
AllignPointCloud::setInputSource(boost::shared_ptr<PointCloudI> cloud_source)
{
    this->cloud_source = cloud_source;
}

void
AllignPointCloud::setInputTarget(boost::shared_ptr<PointCloudI> cloud_target)
{
    this->cloud_target = cloud_target;
}

boost::shared_ptr<QDialog>
AllignPointCloud::getDialog()
{
    return this->allign_dialog.lock();
}



void
AllignPointCloud::extractZSlice(boost::shared_ptr<PointCloudI> cloud, boost::shared_ptr<PointCloudI> base, float height)
{
    pcl::PassThrough<PointI> pass;
    pass.setInputCloud (cloud);
    pass.setFilterFieldName ("z");
    pass.setFilterLimits (height, height+slice_thickness);
    pass.filter (*base);
}



PointCloudI::Ptr
AllignPointCloud::downsampleCloud(PointCloudI::Ptr cloud, float leaf_size)
{
    boost::shared_ptr<PointCloudI> downsampled (new PointCloudI);
    pcl::VoxelGrid<PointI> sor;
    sor.setInputCloud ( cloud );
    sor.setLeafSize ( leaf_size, leaf_size, leaf_size );
    sor.filter ( *downsampled );
    return downsampled;
}

void
AllignPointCloud::ICP()
{
    *cloud_source = *cloud_final;

    boost::shared_ptr<PointCloudI> downsampled_source (new PointCloudI);
    boost::shared_ptr<PointCloudI> downsampled_target (new PointCloudI);

    downsampled_source = downsampleCloud(cloud_source);
    downsampled_target = downsampleCloud(cloud_target);

    pcl::IterativeClosestPoint<PointI, PointI> icp;
    icp.setInputSource(downsampled_source);
    icp.setInputTarget(downsampled_target);
    icp.setMaximumIterations (150);
    icp.setMaxCorrespondenceDistance(0.06);
    icp.setTransformationEpsilon(1e-8);
    icp.setEuclideanFitnessEpsilon(1e-5);
    icp.align(*cloud_final);

    if (icp.hasConverged ())
    {

        QString str("\nICP has converged, score is ");
        str.append(QString::number(icp.getFitnessScore ())).append("\n The transformation Matrix : \n");
        Eigen::Matrix4f transformation_matrix = Eigen::Matrix4f::Identity ();
        transformation_matrix *= icp.getFinalTransformation();
        std::stringstream stream;
        stream << transformation_matrix << "\n";
        str.append(QString::fromStdString(stream.str()));
        result_str = str;
        pcl::transformPointCloud(*cloud_source,*cloud_final, transformation_matrix);
    }
    else
    {
        result_str = QString("\n ICP has not converged. \n");
    }
}

PointCloudI::Ptr
AllignPointCloud::getSource()
{
    return cloud_source;
}

PointCloudI::Ptr
AllignPointCloud::getTarget()
{
    return cloud_target;
}

PointCloudI::Ptr
AllignPointCloud::getFinal()
{
    return cloud_final;
}

void
AllignPointCloud::setInputFinal(PointCloudI::Ptr cloud)
{
    cloud_final = cloud;
}
