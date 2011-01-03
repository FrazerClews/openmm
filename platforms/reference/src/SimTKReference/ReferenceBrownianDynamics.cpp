
/* Portions copyright (c) 2006-2008 Stanford University and Simbios.
 * Contributors: Peter Eastman, Pande Group
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS, CONTRIBUTORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <cstring>
#include <sstream>

#include "../SimTKUtilities/SimTKOpenMMCommon.h"
#include "../SimTKUtilities/SimTKOpenMMLog.h"
#include "../SimTKUtilities/SimTKOpenMMUtilities.h"
#include "ReferenceBrownianDynamics.h"

#include <cstdio>

using std::vector;
using OpenMM::RealVec;

/**---------------------------------------------------------------------------------------

   ReferenceBrownianDynamics constructor

   @param numberOfAtoms  number of atoms
   @param deltaT         delta t for dynamics
   @param friction       friction coefficient
   @param temperature    temperature

   --------------------------------------------------------------------------------------- */

ReferenceBrownianDynamics::ReferenceBrownianDynamics( int numberOfAtoms,
                                                          RealOpenMM deltaT, RealOpenMM friction,
                                                          RealOpenMM temperature ) : 
           ReferenceDynamics( numberOfAtoms, deltaT, temperature ), friction( friction ) {

   // ---------------------------------------------------------------------------------------

   static const char* methodName      = "\nReferenceBrownianDynamics::ReferenceBrownianDynamics";

   static const RealOpenMM zero       =  0.0;
   static const RealOpenMM one        =  1.0;

   // ---------------------------------------------------------------------------------------

   if( friction <= zero ){

      std::stringstream message;
      message << methodName;
      message << " input frction value=" << friction << " is invalid -- setting to 1.";
      SimTKOpenMMLog::printError( message );

      this->friction = one;
     
   }
   xPrime.resize(numberOfAtoms);
   inverseMasses.resize(numberOfAtoms);
}

/**---------------------------------------------------------------------------------------

   ReferenceBrownianDynamics destructor

   --------------------------------------------------------------------------------------- */

ReferenceBrownianDynamics::~ReferenceBrownianDynamics( ){

   // ---------------------------------------------------------------------------------------

   // static const char* methodName = "\nReferenceBrownianDynamics::~ReferenceBrownianDynamics";

   // ---------------------------------------------------------------------------------------

}

/**---------------------------------------------------------------------------------------

   Get the friction coefficient

   @return friction

   --------------------------------------------------------------------------------------- */

RealOpenMM ReferenceBrownianDynamics::getFriction( void ) const {

   // ---------------------------------------------------------------------------------------

   // static const char* methodName  = "\nReferenceBrownianDynamics::getFriction";

   // ---------------------------------------------------------------------------------------

   return friction;
}

/**---------------------------------------------------------------------------------------

   Update -- driver routine for performing Brownian dynamics update of coordinates
   and velocities

   @param numberOfAtoms       number of atoms
   @param atomCoordinates     atom coordinates
   @param velocities          velocities
   @param forces              forces
   @param masses              atom masses

   @return ReferenceDynamics::DefaultReturn

   --------------------------------------------------------------------------------------- */

int ReferenceBrownianDynamics::update( int numberOfAtoms, vector<RealVec>& atomCoordinates,
                                          vector<RealVec>& velocities,
                                          vector<RealVec>& forces, vector<RealOpenMM>& masses ){

   // ---------------------------------------------------------------------------------------

   static const char* methodName      = "\nReferenceBrownianDynamics::update";

   static const RealOpenMM zero       =  0.0;
   static const RealOpenMM one        =  1.0;

   // ---------------------------------------------------------------------------------------

   // first-time-through initialization

   if( getTimeStep() == 0 ){

      std::stringstream message;
      message << methodName;
      int errors = 0;

      // invert masses

      for( int ii = 0; ii < numberOfAtoms; ii++ ){
         if( masses[ii] <= zero ){
            message << "mass at atom index=" << ii << " (" << masses[ii] << ") is <= 0" << std::endl;
            errors++;
         } else {
            inverseMasses[ii] = one/masses[ii];
         }
      }

      // exit if errors

      if( errors ){
         SimTKOpenMMLog::printError( message );
      }
   }
   
   // Perform the integration.
   
   const RealOpenMM noiseAmplitude = static_cast<RealOpenMM>( sqrt(2.0*BOLTZ*getTemperature()*getDeltaT()/getFriction()) );
   const RealOpenMM forceScale = getDeltaT()/getFriction();
   for (int i = 0; i < numberOfAtoms; ++i) {
       for (int j = 0; j < 3; ++j) {
           xPrime[i][j] = atomCoordinates[i][j] + forceScale*inverseMasses[i]*forces[i][j] + noiseAmplitude*SQRT(inverseMasses[i])*SimTKOpenMMUtilities::getNormallyDistributedRandomNumber();
       }
   }
   ReferenceConstraintAlgorithm* referenceConstraintAlgorithm = getReferenceConstraintAlgorithm();
   if( referenceConstraintAlgorithm )
      referenceConstraintAlgorithm->apply( numberOfAtoms, atomCoordinates, xPrime, inverseMasses );
   
   // Update the positions and velocities.
   
   RealOpenMM velocityScale = static_cast<RealOpenMM>( 1.0/getDeltaT() );
   for (int i = 0; i < numberOfAtoms; ++i) {
       for (int j = 0; j < 3; ++j) {
           velocities[i][j] = velocityScale*(xPrime[i][j] - atomCoordinates[i][j]);
           atomCoordinates[i][j] = xPrime[i][j];
       }
   }

   incrementTimeStep();

   return ReferenceDynamics::DefaultReturn;

}
