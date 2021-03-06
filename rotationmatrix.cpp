/*

    comet - an actin-based motility simulator
    Copyright (C) 2009 Mark J Dayel

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>

    When using the code in academic work please cite:

      Dayel MJ, Akin O, Landeryou M, Risca V, Mogilner A, et al. (2009) 
      In Silico Reconstitution of Actin-Based Symmetry Breaking and Motility. 
      PLoS Biol 7(9):e1000201. doi:10.1371/journal.pbio.1000201

    and include any modifications to the source code with the published work.

*/


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

rotationmatrix::rotationmatrix(const double &angle, const projection &a)
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

void rotationmatrix::rotatematrix(const double &angle, const projection &a)
{
    // create a tmp rotation matrix for this axis

    rotationmatrix tmp;
   
    switch (a)
    {
	    case (xaxis):
	    {
	        tmp.xx =  1;			tmp.xy =  0;			tmp.xz =  0;
	        tmp.yx =  0;			tmp.yy =  cos(angle);	tmp.yz =  sin(angle);
	        tmp.zx =  0;			tmp.zy = -sin(angle);	tmp.zz =  cos(angle);
	        break;
	    }
	    case (yaxis):
	    {
	        tmp.xx =  cos(angle);	tmp.xy =  0;			tmp.xz = -sin(angle);
	        tmp.yx =  0;			tmp.yy =  1;			tmp.yz =  0;
	        tmp.zx =  sin(angle);	tmp.zy =  0;			tmp.zz =  cos(angle);
	        break;
	    }
	    default:
	    {
	        tmp.xx =  cos(angle);	tmp.xy =  sin(angle);	tmp.xz =  0;
	        tmp.yx = -sin(angle);	tmp.yy =  cos(angle);	tmp.yz =  0;
	        tmp.zx =  0;			tmp.zy =  0;			tmp.zz =  1;
	    } 
    }
    
    // rotate the matrix by tmp
    rotatematrix(tmp);
    
}

rotationmatrix rotationmatrix::inverse()
{                                                             
    rotationmatrix adj;

    // determinant
    double det =  xx * ( yy*zz - yz*zy )
                + yx * ( zy*xz - xy*zz )
                + zx * ( xy*yz - yy*xz );
    // adjunct
    adj.xx = yy*zz - yz*zy;     adj.xy = xz*zy - xy*zz;     adj.xz = xy*yz - xz*yy;
    adj.yx = yz*zx - yx*zz; 	adj.yy = xx*zz - xz*zx; 	adj.yz = xz*yx - xx*yz;
    adj.zx = yx*zy - yy*zx; 	adj.zy = xy*zx - xx*zy; 	adj.zz = xx*yy - xy*yx;

    return adj * (1/det);   // inverse is adjunct/determinant
}

void rotationmatrix::rotatematrix(const rotationmatrix& rotmatrix)
{   // multiplies matrix
    double tmp_xx,tmp_xy,tmp_xz;
    double tmp_yx,tmp_yy,tmp_yz;
    double tmp_zx,tmp_zy,tmp_zz;

    tmp_xx = xx * rotmatrix.xx + yx * rotmatrix.xy + zx * rotmatrix.xz;
    tmp_yx = xx * rotmatrix.yx + yx * rotmatrix.yy + zx * rotmatrix.yz; 
    tmp_zx = xx * rotmatrix.zx + yx * rotmatrix.zy + zx * rotmatrix.zz; 
    
    tmp_xy = xy * rotmatrix.xx + yy * rotmatrix.xy + zy * rotmatrix.xz;
    tmp_yy = xy * rotmatrix.yx + yy * rotmatrix.yy + zy * rotmatrix.yz; 
    tmp_zy = xy * rotmatrix.zx + yy * rotmatrix.zy + zy * rotmatrix.zz;
    
    tmp_xz = xz * rotmatrix.xx + yz * rotmatrix.xy + zz * rotmatrix.xz;
    tmp_yz = xz * rotmatrix.yx + yz * rotmatrix.yy + zz * rotmatrix.yz; 
    tmp_zz = xz * rotmatrix.zx + yz * rotmatrix.zy + zz * rotmatrix.zz;
    
    xx = tmp_xx;
    yx = tmp_yx;
    zx = tmp_zx; 
    
    xy = tmp_xy;
    yy = tmp_yy;
    zy = tmp_zy;
    
    xz = tmp_xz;
    yz = tmp_yz; 
    zz = tmp_zz;  
}

rotationmatrix& rotationmatrix::operator*=(const rotationmatrix& rotmatrix)
{  
    rotatematrix(rotmatrix);
    return *this;
}


void rotationmatrix::rotatematrix(const double& x_angle, const double& y_angle, const double& z_angle)
{
    rotatematrix(x_angle, xaxis);
    rotatematrix(y_angle, yaxis);
    rotatematrix(z_angle, zaxis);
}

//void rotationmatriz::rotatematrix(const vect & v) // work in progress---attempt to rotate along axis vector, angle proportional to length
//{
//   
//}


//void rotationmatrix::rotatematrixrevorder(const double& x_angle, const double& y_angle, const double& z_angle)
//{   // not commutative, so have to reverse the order when calculating inverse
//    // todo: matrix inverse function?
//    rotatematrix(z_angle, zaxis);
//    rotatematrix(y_angle, yaxis);
//    rotatematrix(x_angle, xaxis);
//}

void rotationmatrix::getangles(double& x_angle, double& y_angle, double& z_angle) const 
{  
    y_angle = -atan2(zx,calcdist(xx,yx));
    z_angle =  atan2(yx/cos(y_angle),xx/cos(y_angle));
    x_angle	=  atan2(zy/cos(y_angle),zz/cos(y_angle));
}

void rotationmatrix::getaxis_and_angle(double& x, double& y, double& z, double& angle) const 
{  // todo: check these are right way around
    
    angle = acos ( ( xx + yy + zz - 1) / 2 );

    double l = sqrt( (zy - yz)*(zy - yz) + (xz - zx)*(xz - zx) + (yx - xy)*(yx - xy) );

    x = (yz - zy) / l;
    y = (zx - xz) / l;
    z = (xy - yx) / l;

}


ostream& operator<<(ostream &ostm, rotationmatrix const &rm){

    ostm << rm.xx << " " << rm.xy << " " << rm.xz << " "
		 << rm.yx << " " << rm.yy << " " << rm.yz << " "
		 << rm.zx << " " << rm.zy << " " << rm.zz;
    return ostm;
}

istream& operator>>(istream& istm, rotationmatrix &rm){
    
	istm >> rm.xx  >> rm.xy  >> rm.xz 
		 >> rm.yx  >> rm.yy  >> rm.yz 
		 >> rm.zx  >> rm.zy >> rm.zz;

    return istm;
}
