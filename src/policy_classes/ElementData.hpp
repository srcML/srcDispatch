/**
 * @file ElementData.hpp
 *
 * Base class for all policy's data elements.
 *
 * @author Michael John Decker, Ph.D. <mdecke@bgsu.edu>
 */

#ifndef INCLUDED_ELEMENT_DATA_HPP
#define INCLUDED_ELEMENT_DATA_HPP

namespace srcDispatch {

    struct ElementData {

        DeltaElement<unsigned int> startLineNumber;
        DeltaElement<unsigned int> endLineNumber;

        virtual ~ElementData() {}
    };

}

#endif
