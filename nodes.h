/*
    comet - an actin-based motility simulator
    Copyright (C) 2005 Mark J Dayel

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

#ifndef nodes_H
#define nodes_H

#include "stdafx.h"
//#include "actin.h"
#include "vect.h"
#include "Colour.h"
#include "links.h"

class links;
class Colour;
class actin;

class nodes: public vect
{
public:

    NODEGRIDTYPE<nodes*>* nodegridptr;
	
	bool onseg;
    bool polymer;
    bool harbinger;
	bool stucktonucleator;
	bool move_harbinger_this_time;
    bool testnode;

    int nodenum;
    int creation_iter_num;
    

    short int testsurface;  // surface number that node is attached to

    short int threadnum;    

	short int gridx, gridy, gridz;

    vect delta, delta_sum;
	vect link_force_vec;
	vect rep_force_vec;

	vect unit_vec_posn;  // this is kept up-to-date in the updategrid() function


	vect nuc_repulsion_displacement_vec;
 	vect nucleator_stuck_position;
	
	vect viscosity_velocity_sum;

    vect nucleator_link_force;	// force on nodes by the link to the nucleator 
								// again merely for display purposes
								// segmented in segments.cpp
								// nucleator attachements are dealt with in actin::nucleator_node_interactions()

    double pressure;

	double linkforce_transverse, linkforce_radial,  
			 repforce_transverse, repforce_radial,
			 links_broken;

    //double linkenergy_transverse, linkenergy_radial,
	//		 repenergy_transverse, repenergy_radial,
	//		 linksenergy_broken;

    //double initial_repulsive_energy;
	
	vect nucleator_impacts;	// keeps track of ejections of node from nucleator
								// merely for display of ejection forces
								// segmented in segments.cpp
								// not used in moving node, nucleator etc.
								// that is done directly at the mo.

	//Colour colour;

	
	double dist_from_surface;

	double viscosity_velocity_unweight;

    vector <links> listoflinks;



    int savelinks(ofstream * outstream);
    
	nodes(void);
	~nodes(void);
	nodes(const double& set_x, const double& set_y,const double& set_z);

	bool depolymerize(void);
	bool polymerize(const double& set_x, const double& set_y, const double& set_z);
    bool polymerize(const vect& v);
	int save_data(ofstream &ofstr);
	bool load_data(ifstream &ifstr);

	void addlink(nodes& linkto, const double& dist);
	void removelink(const nodes* linkednode);

	void updategrid(void);
	void removefromgrid(void);
	void addtogrid();
	void setgridcoords(void);

    vect posnoflastgridupdate;

    //bool unit_vec_correct;
    vect pos_in_nuc_frame; //, previous_pos_in_nuc_frame;

	inline void applyforces() 
	{	
        //unit_vec_correct = false;

	    if (VISCOSITY)
	    {
		    // simple average weighted by VISCOSITY_FACTOR
		    //
		    //delta = (delta + (viscosity_velocity_sum * VISCOSITY_FACTOR )) / 
		    //	                     ( 1 + VISCOSITY_FACTOR + mymax(VISCOSITY_EDGE_FACTOR, viscosity_velocity_unweight));

		    delta = ( (link_force_vec + rep_force_vec ) * NODE_FORCE_TO_DIST * NON_VISC_WEIGHTING + 
			        (viscosity_velocity_sum	* VISCOSITY_FACTOR )) / 
			                         ( NON_VISC_WEIGHTING + VISCOSITY_FACTOR * mymax(VISCOSITY_EDGE_FACTOR, viscosity_velocity_unweight));
        } else
        {
	        delta = (link_force_vec + rep_force_vec)				
                    * NODE_FORCE_TO_DIST;
        }

        *this += delta;
        delta_sum += delta;

        setunitvec();  // we've moved the node, so reset the unit vector
        clearforces();   // and clear force sums we just used

        // note: remember to do this clearing too when switch clearing harbinger flag
        //       in crosslinknewnodes()
    }

	inline void clearforces()
	{   // this is called every iteration (as opposed to clearstats() which builds up until the end of a frame)
		
	    if (VISCOSITY)
	    {
		    viscosity_velocity_unweight = 0.0;
		    viscosity_velocity_sum.zero();
	    }

	    rep_force_vec.zero();
	    link_force_vec.zero();                                    

	    pressure = 0;

	}



	inline void getdirectionalmags(const vect &v, double &dotmag, double &crossmag) const
	{
		dotmag = fabs(unit_vec_posn.dot(v));
		crossmag = v.length() - dotmag;
	}

	inline void adddirectionalmags(const vect &v, double &dotmag, double &crossmag) const
	{   // add components of v into dotmag a crossmag vectors
		
		//double mag_in_radial_dirn = fabs(unit_vec_posn.dot(v));

        dotmag   += fabs(v.dot(unit_vec_posn));
		crossmag += v.cross(unit_vec_posn).length();

        // temporarily hijack this function for the x,z components (for capsule plotting---not worth breaking the save file format)
                                           // note this will not be useful after symmetry is broken 
                                           // (because this is the simulation x frame, not the bead or camera frame)
                                           // ...this is OK since we only need it for the pre sym break info
                                           // we could put this in the bead frame, but it's extra calculation, and we don't need it

        //dotmag   += fabs( v.cross(unit_vec_posn).x);  // component into picture
		//crossmag +=       v.cross(unit_vec_posn).cross(vect(1,0,0)).length() ;

	}

	inline void clearstats()
	{
		// these stats are built up over many iterations, and cleared every readout frame

		linkforce_transverse = 
		linkforce_radial     = 
		repforce_transverse  = 
		repforce_radial      = 
		links_broken	     = 0.0;
 		nucleator_impacts.zero();

		nucleator_link_force.zero();

        delta_sum.zero();

        for (vector <links>::iterator i_link  = listoflinks.begin();
                                      i_link != listoflinks.end();
                                    ++i_link )
		{
            i_link->clearstats();
        }

//        linkenergy_transverse = 
//       linkenergy_radial     =
//		repenergy_transverse  =
//       repenergy_radial      =
//		linksenergy_broken    = 0.0;

	}


    void setunitvec();

};

#endif
