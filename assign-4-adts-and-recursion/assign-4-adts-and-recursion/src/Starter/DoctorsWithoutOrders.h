#ifndef DoctorsWithoutOrders_Included
#define DoctorsWithoutOrders_Included

#include "RecursionToTheRescue.h"
#include "vector.h"
#include <istream>

/**
 * Type representing a test case for the Doctors Without Orders problem.
 */
struct HospitalTestCase {
    Vector<Doctor>  doctors;
    Vector<Patient> patients;
    bool isPossible;
};

/**
 * Given a stream containing a hospital test case file, reads the
 * data from that file and returns a HospitalTestCase containing
 * the relevant information.
 *
 * @param source The stream to read from.
 * @return The test case from that file.
 * @throws ErrorException If the test case is malformed.
 */
HospitalTestCase loadHospitalTestCase(std::istream& source);

#endif
