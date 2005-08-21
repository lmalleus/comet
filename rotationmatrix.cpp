#include "stdafx.h"
#include "rotationmatrix.h"

rotationmatrix::rotationmatrix(void)
{
	settoidentity();
}

rotationmatrix::~rotationmatrix(void)
{
}

void rotationmatrix::settoidentity(void)
{
	 xx = yy = zz = 1;
    xy = xz = yx = yz = zx = zy = 0;
}

rotationmatrix::rotationmatrix(double angle, axis a)
{
    switch (a)
    {
	case (xaxis):
	{
	    xx =  1;			xy =  0;			xz =  0;
	    yx =  0;			yy =  cos(angle);	yz =  sin(angle);
	    zx =  0;			zy = -sin(angle);	zz =  cos(angle);
	    break;
	}
	case (yaxis):
	{
	    xx =  cos(angle);	xy =  0;			xz = -sin(angle);
	    yx =  0;			yy =  1;			yz =  0;
	    zx =  sin(angle);	zy =  0;			zz =  cos(angle);
	    break;
	}
	default:
	{
	    xx =  cos(angle);	xy =  sin(angle);	xz =  0;
	    yx = -sin(angle);	yy =  cos(angle);	yz =  0;
	    zx =  0;			zy =  0;			zz =  1;
	} 
    }
}

void rotationmatrix::rotatematrix(double angle, axis a)
{
    double tmp_xx,tmp_xy,tmp_xz;
    double tmp_yx,tmp_yy,tmp_yz;
    double tmp_zx,tmp_zy,tmp_zz;
    
    double tmp2_xx,tmp2_xy,tmp2_xz;
    double tmp2_yx,tmp2_yy,tmp2_yz;
    double tmp2_zx,tmp2_zy,tmp2_zz;
    
    switch (a)
    {
	case (xaxis):
	{
	    tmp_xx =  1;			tmp_xy =  0;			tmp_xz =  0;
	    tmp_yx =  0;			tmp_yy =  cos(angle);	tmp_yz =  sin(angle);
	    tmp_zx =  0;			tmp_zy = -sin(angle);	tmp_zz =  cos(angle);
	    break;
	}
	case (yaxis):
	{
	    tmp_xx =  cos(angle);	tmp_xy =  0;			tmp_xz = -sin(angle);
	    tmp_yx =  0;			tmp_yy =  1;			tmp_yz =  0;
	    tmp_zx =  sin(angle);	tmp_zy =  0;			tmp_zz =  cos(angle);
	    break;
	}
	default:
	{
	    tmp_xx =  cos(angle);	tmp_xy =  sin(angle);	tmp_xz =  0;
	    tmp_yx = -sin(angle);	tmp_yy =  cos(angle);	tmp_yz =  0;
	    tmp_zx =  0;			tmp_zy =  0;			tmp_zz =  1;
	} 
    }
    
    // rotate the matrix by tmp_
    
    tmp2_xx = xx * tmp_xx + yx * tmp_xy + zx * tmp_xz;
    tmp2_yx = xx * tmp_yx + yx * tmp_yy + zx * tmp_yz; 
    tmp2_zx = xx * tmp_zx + yx * tmp_zy + zx * tmp_zz; 
    
    tmp2_xy = xy * tmp_xx + yy * tmp_xy + zy * tmp_xz;
    tmp2_yy = xy * tmp_yx + yy * tmp_yy + zy * tmp_yz; 
    tmp2_zy = xy * tmp_zx + yy * tmp_zy + zy * tmp_zz;
    
    tmp2_xz = xz * tmp_xx + yz * tmp_xy + zz * tmp_xz;
    tmp2_yz = xz * tmp_yx + yz * tmp_yy + zz * tmp_yz; 
    tmp2_zz = xz * tmp_zx + yz * tmp_zy + zz * tmp_zz;
    
    xx = tmp2_xx;
    yx = tmp2_yx;
    zx = tmp2_zx; 
    
    xy = tmp2_xy;
    yy = tmp2_yy;
    zy = tmp2_zy;
    
    xz = tmp2_xz;
    yz = tmp2_yz; 
    zz = tmp2_zz;
    
}

void rotationmatrix::rotatematrix(const double& x_angle, const double& y_angle, const double& z_angle)
{
    rotatematrix(x_angle, xaxis);
    rotatematrix(y_angle, yaxis);
    rotatematrix(z_angle, zaxis);
}

void rotationmatrix::getangles(double& x_angle, double& y_angle, double& z_angle)
{  // todo: check these are right way around
    y_angle = -atan2(zx,calcdist(xx,yx));
    z_angle =  atan2(yx/cos(y_angle),xx/cos(y_angle));
    x_angle	=  atan2(zy/cos(y_angle),zz/cos(y_angle));
}


ostream& operator<<(ostream &ostm, rotationmatrix const &rm){
    ostm << rm.xx << " " << rm.xy << " " << rm.xz << " "
	 << rm.yx << " " << rm.yy << " " << rm.yz << " "
	 << rm.zx << " " << rm.zy << " " << rm.zz;
    return ostm;
}

istream& operator>>(istream& istm, rotationmatrix &rm){
    istm >> rm.xx >> rm.xy >> rm.xz
	 >> rm.yx >> rm.yy >> rm.yz
	 >> rm.zx >> rm.zy >> rm.zz;
    return istm;
}
