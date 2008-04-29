
/* Portions copyright (c) 2006-2008 Stanford University and Simbios.
 * Contributors: Pande Group
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

#ifndef __ReferenceBrownianDynamics_H__
#define __ReferenceBrownianDynamics_H__

#include "ReferenceDynamics.h"

// ---------------------------------------------------------------------------------------

class ReferenceBrownianDynamics : public ReferenceDynamics {

   private:

      enum TwoDArrayIndicies { xPrime2D, Max2DArrays };
      enum OneDArrayIndicies { InverseMasses, Max1DArrays };

      RealOpenMM friction;
      
   public:

      /**---------------------------------------------------------------------------------------
      
         Constructor

         @param numberOfAtoms  number of atoms
         @param deltaT         delta t for dynamics
         @param friction       friction coefficient
         @param temperature    temperature
      
         --------------------------------------------------------------------------------------- */

       ReferenceBrownianDynamics( int numberOfAtoms, RealOpenMM deltaT, RealOpenMM friction, RealOpenMM temperature );

      /**---------------------------------------------------------------------------------------
      
         Destructor
      
         --------------------------------------------------------------------------------------- */

       ~ReferenceBrownianDynamics( );

      /**---------------------------------------------------------------------------------------
      
         Get the friction coefficient
      
         @return friction
      
         --------------------------------------------------------------------------------------- */
      
      RealOpenMM getFriction( void ) const;
      
      /**---------------------------------------------------------------------------------------
      
         Print parameters
      
         @param message message

         @return ReferenceDynamics::DefaultReturn
      
         --------------------------------------------------------------------------------------- */
      
      int printParameters( std::stringstream& message ) const;
      
      /**---------------------------------------------------------------------------------------
      
         Update
      
         @param numberOfAtoms       number of atoms
         @param atomCoordinates     atom coordinates
         @param velocities          velocities
         @param forces              forces
         @param masses              atom masses
      
         @return ReferenceDynamics::DefaultReturn
      
         --------------------------------------------------------------------------------------- */
     
      int update( int numberOfAtoms, RealOpenMM** atomCoordinates,
                  RealOpenMM** velocities, RealOpenMM** forces, RealOpenMM* masses );
      
      /**---------------------------------------------------------------------------------------
      
         Write state
      
         @param numberOfAtoms       number of atoms
         @param atomCoordinates     atom coordinates
         @param velocities          velocities
         @param forces              forces
         @param masses              atom masses
         @param state               0 if initial state; otherwise nonzero
         @param baseFileName        base file name
      
         @return ReferenceDynamics::DefaultReturn
      
         --------------------------------------------------------------------------------------- */
      
      int writeState( int numberOfAtoms, RealOpenMM** atomCoordinates,
                      RealOpenMM** velocities, RealOpenMM** forces, RealOpenMM* masses,
                      int state, const std::string& baseFileName ) const;
      
};

// ---------------------------------------------------------------------------------------

#endif // __ReferenceBrownianDynamics_H__
