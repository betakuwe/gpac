/******************************************************************************

                        (C) Copyright Fraunhofer IIS (2014)
                               All Rights Reserved

   $Id$
   Author(s):   Thomas Blender (fix-point version)
   Description: Quickhull algorithm to calculate triangulation meshes
                of the convex hull around the given loudspeaker setup

   This software and/or program is protected by copyright law and international
   treaties. Any reproduction or distribution of this software and/or program,
   or any portion of it, may result in severe civil and criminal penalties, and
   will be prosecuted to the maximum extent possible under law.

******************************************************************************/

#ifndef QuickHullVBAP_QuickHull_h
#define QuickHullVBAP_QuickHull_h


#include "cartesianMath.h"
#include "CDK_core.h"
#include "CDK_matrixCalloc.h"
#include <stdlib.h>     /* qsort */


#define FX_DBL_DEGREE2INT 11930464          /* 11930464,7 corresponds to 1° in FIXP_DBL domain. Bring down because of rounding reasons in init_vertex*/


#define GVBAPRENDERER_VERTEX_LIST_MAX_SIZE   28 + 2  /* 2 is a value of safety */
#define GVBAPRENDERER_TRIANGLE_LIST_MAX_SIZE 49 + 2  /* 2 is a value of safety */


#define D2F(val)  FL2FXCONST_DBL(val/180.0) /* convert a degree value to fixpoint */

/* quadrilateral front and rear subsets */
const FIXP_DBL rangeOfSubsetA[16] = { D2F(+23.0),  D2F(+37.0), D2F( -9.0), D2F(+20.0),  /* CH_M_L030 */
                                      D2F(-37.0),  D2F(-23.0), D2F( -9.0), D2F(+20.0),  /* CH_M_R030 */
                                      D2F(-37.0),  D2F(-11.0), D2F(+21.0), D2F(+60.0),  /* CH_U_R030 */
                                      D2F(+11.0),  D2F(+37.0), D2F(+21.0), D2F(+60.0)}; /* CH_U_L030 */

const FIXP_DBL rangeOfSubsetB[16] = { D2F(+38.0),  D2F(+52.0), D2F( -9.0), D2F(+20.0),  /* CH_M_L045 */
                                      D2F(-52.0),  D2F(-38.0), D2F( -9.0), D2F(+20.0),  /* CH_M_R045 */
                                      D2F(-66.0),  D2F(-38.0), D2F(+21.0), D2F(+60.0),  /* CH_U_R045 */
                                      D2F(+38.0),  D2F(+66.0), D2F(+21.0), D2F(+60.0)}; /* CH_U_L045 */

const FIXP_DBL rangeOfSubsetC[16] = {D2F(+101.0), D2F(+124.0), D2F(-45.0), D2F(+20.0),  /* CH_M_L110 */
                                     D2F(+101.0), D2F(+124.0), D2F(+21.0), D2F(+60.0),  /* CH_U_L110 */
                                     D2F(-124.0), D2F(-101.0), D2F(+21.0), D2F(+60.0),  /* CH_U_R110 */
                                     D2F(-124.0), D2F(-101.0), D2F(-45.0), D2F(+20.0)}; /* CH_M_R110 */

const FIXP_DBL rangeOfSubsetD[16] = {D2F(+125.0), D2F(+142.0), D2F(-45.0), D2F(+20.0),  /* CH_M_L135 */
                                     D2F(+125.0), D2F(+157.0), D2F(+21.0), D2F(+60.0),  /* CH_U_L135 */
                                     D2F(-157.0), D2F(-125.0), D2F(+21.0), D2F(+60.0),  /* CH_U_R135 */
                                     D2F(-142.0), D2F(-125.0), D2F(-45.0), D2F(+20.0)}; /* CH_M_R135 */

/*
 * @brief Vertex struct defined by its three Cartesian coordinates
 * @param xyz       Cartesian coordinate
 * @param index     sorting Index
 * @param sph       Azimuth
 */
typedef struct _vertex {

  PointCartesian xyz;
  int index;
  PointSpherical sph;

} vertex;

/*
 * @brief Vertex List
 * @param size      size of List
 * @param element   vertex containing:
 *                    xyz       Cartesian coordinates
 *                    index     sorting Index
 *                    sph       Azimuth
 */
typedef struct _vertexList {

  int size;
  int limit;
  vertex element[GVBAPRENDERER_VERTEX_LIST_MAX_SIZE];

} vertexList;



/*
 * @brief Triangle struct defined by a set of three Vertex indices
 * @param three corner vertices
 */
typedef struct _triangle {
    int index[3];
} triangle;

/*
 * @brief Triangle List
 * @param size      size of List
 * @param element   three corner vertices
 */
typedef struct _triangleList {

  int size;
  int limit;
  triangle element[GVBAPRENDERER_TRIANGLE_LIST_MAX_SIZE];

} triangleList;


/*
 * @brief Stores surround speaker below 45° elevation
 * @param azi       Azimuth
 * @param index     Speaker index in original vertexList
 */
typedef struct _surroundList {

  FIXP_DBL azi;
  int index;

} surroundList;





/******************************************
 *          API Functions                 *
 ******************************************/


/*
 * @brief Fills a vertexList vL with given a number of speakers and speaker positions (degree)
 * @param vL Output Vertex list with spherical and Cartesian speaker positions and a sorting index
 * @param numSpk Number of speakers
 * @param az Azimuth   fixpoint value of speaker position. Value is mapped to -1 ... 1 for -180° ... 180° or -pi .. pi
 * @param el Elevation fixpoint value of speaker position. Value is mapped to -1 ... 1 for -180° ... 180° or -pi .. pi
 */
void qh_gen_VertexList(int numSpk, FIXP_DBL* az, FIXP_DBL* el, vertexList* vL);


/*
 * @brief Fills a triangleList tL with speaker triangles by given vertexList vL
 *        and generates downmix matrix if ghost speakers where added
 * @param vL Input Vertex list with spherical and Cartesian speaker positions and a sorting index
 * @param tL Output triangleList
 * @return error code
 */
int qh_sphere_triangulation(vertexList* vL, triangleList* tL, FIXP_DBL ***downmixMatrix);






/******************************************
 *        list functions                  *
 ******************************************/

/*
 * @brief reset initial values of vertexList vL
 */
void resetVertexList(vertexList *vL);

/*
 * @brief reset initial values of triangleList tL
 */
void resetTriangleList(triangleList *tL);

/*
 * @brief adds element v to a given vertexList vL
 */
void addVertexToList(vertexList* vL, vertex v);

/*
 * @brief adds element t to a given triangleList tL
 */
void addTriangleToList(triangleList* tL, triangle t);



/*
 * @brief Sort indices of an edge in an ascending fashion
 * @param edgeList is a pointer to the position of list where the next 2 elements will be sorted
 */
void sortEdgeList(int* edgeList);

/*
 * @brief Add new vertex vtx to a convex hull. The convex hull is given by the triangles tL. The plane orientation has to be counter clockwise and the result
 *        is guaranteed to be counter clockwise as well. The given convex hull has to cover at least a hemisphere.
 * @param vL Vertex list with coordinates.
 * @param tL List of indices of plane vertices.
 * @param vtx New vertex.
 */
void add_vertex(vertexList* vL, triangleList* tL, int vtx);

/*
 * @brief Initialize vertex including the sorting index.
 * @param azimuth Azimuth angle in degree
 * @param elevation Elevation angle in degree
 */
vertex init_vertex(FIXP_DBL azimuth, FIXP_DBL elevation);

/*
 *  @brief Add Speakers by the 5 rules in the definition
 *  @param vertexList vL of real speaker positions
 *  @return modified vertexList with added Ghosts
 *  @return indices of initial pentagon (length have to be 5)
 */
int add_imaginary_speakers(vertexList* vL, int* pentagon);



/*
 * \brief Tests whether a subset exists and if it is empty or not.
 *        If an empty subset was found a ghost speaker will be added
 *
 * \param vL vertexList of all speakers
 * \param subsetRange is one of the defined subsetrange in quickHull.h
 * \param frontSide indicates whether the subset is a front or back side subset
 * Returns 0 if no ghost speaker was added
 * Returns 1 if a ghost speaker was added
 */
int addGhostToSubset(vertexList* vL, const FIXP_DBL* subsetRange, bool frontSide);


/*
 * @brief calculates the distance between a point p and a plane spanned by the points x1, x2, x3
 * @param x1, x2, x3, p Cartesian points. All exponents have to be the same.
 *
 * @return scalar value of distance with sign as orientation. Result has a exponent of 1.
 */
FIXP_DBL distPointPlane(PointCartesian x1, PointCartesian x2, PointCartesian x3, PointCartesian p);



#endif

