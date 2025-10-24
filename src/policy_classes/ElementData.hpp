/**
 * @file ElementData.hpp
 *
 * Base class for all policy's data elements.
 *
 * @author Michael John Decker, Ph.D. <mdecke@bgsu.edu>
 */

#ifndef INCLUDED_ELEMENT_DATA_HPP
#define INCLUDED_ELEMENT_DATA_HPP

#include <Position.hpp>

namespace srcDispatch {

    struct ElementData {

        DeltaElement<Position> startPosition;
        DeltaElement<Position> endPosition;

        ElementData() : startPosition(), endPosition() {
        }

        virtual ~ElementData() {
        }
    };

}

#endif
