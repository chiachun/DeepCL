// Copyright Hugh Perkins 2014 hughperkins at gmail
//
// This Source Code Form is subject to the terms of the Mozilla Public License, 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#include "InputLayer.h"

using namespace std;

#undef VIRTUAL
#define VIRTUAL 

template< typename T > InputLayer<T>::InputLayer( Layer *previousLayer, InputLayerMaker<T> const*maker ) :
       Layer( previousLayer, maker ),
    batchSize(0),
    allocatedSize(0),
    input(0),
    results(0),
    outputPlanes( maker->getOutputPlanes() ),
    outputBoardSize( maker->getOutputBoardSize() ) {
}
template< typename T > VIRTUAL InputLayer<T>::~InputLayer() {
}
template< typename T > VIRTUAL float *InputLayer<T>::getResults() {
    return results;
}
template< typename T > VIRTUAL ActivationFunction const *InputLayer<T>::getActivationFunction() {
    return new LinearActivation();
}
template< typename T > VIRTUAL bool InputLayer<T>::needsBackProp() {
    return false;
}
template< typename T > VIRTUAL void InputLayer<T>::printOutput() const {
    if( results == 0 ) {
         return;
    }
    for( int n = 0; n < std::min(5,batchSize); n++ ) {
        std::cout << "InputLayer n " << n << ":" << std::endl;
        for( int plane = 0; plane < std::min( 5, outputPlanes); plane++ ) {
            if( outputPlanes > 1 ) std::cout << "    plane " << plane << ":" << std::endl;
            for( int i = 0; i < std::min(5, outputBoardSize); i++ ) {
                std::cout << "      ";
                for( int j = 0; j < std::min(5, outputBoardSize); j++ ) {
                    std::cout << getResult( n, plane, i, j ) << " ";
//results[
//                            n * numPlanes * boardSize*boardSize +
//                            plane*boardSize*boardSize +
//                            i * boardSize +
//                            j ] << " ";
                }
                if( outputBoardSize > 5 ) std::cout << " ... ";
                std::cout << std::endl;
            }
            if( outputBoardSize > 5 ) std::cout << " ... " << std::endl;
        }
        if( outputPlanes > 5 ) std::cout << "   ... other planes ... " << std::endl;
    }
    if( batchSize > 5 ) std::cout << "   ... other n ... " << std::endl;
}
template< typename T > VIRTUAL void InputLayer<T>::print() const {
    printOutput();
}
template< typename T > void InputLayer<T>::in( T const*images ) {
//        std::cout << "InputLayer::in()" << std::endl;
    this->input = images;
//        this->batchStart = batchStart;
//        this->batchEnd = batchEnd;
//        print();
}
template< typename T > VIRTUAL bool InputLayer<T>::needErrorsBackprop() {
    return false;
}
template< typename T > VIRTUAL void InputLayer<T>::setBatchSize( int batchSize ) {
//        std::cout << "inputlayer setting batchsize " << batchSize << std::endl;
    if( batchSize <= allocatedSize ) {
        this->batchSize = batchSize;
        return;
    }
    if( results != 0 ) {
        delete[] results;
    }
    this->batchSize = batchSize;
    this->allocatedSize = batchSize;
    results = new float[batchSize * getOutputCubeSize() ];
}
template< typename T > VIRTUAL void InputLayer<T>::propagate() {
    int totalLinearLength = getResultsSize();
    for( int i = 0; i < totalLinearLength; i++ ) {
        results[i] = input[i];
    }
}
template< typename T > VIRTUAL void InputLayer<T>::backPropErrors( float learningRate, float const *errors ) {
}
template< typename T > VIRTUAL int InputLayer<T>::getOutputBoardSize() const {
    return outputBoardSize;
}
template< typename T > VIRTUAL int InputLayer<T>::getOutputPlanes() const {
    return outputPlanes;
}
template< typename T > VIRTUAL int InputLayer<T>::getOutputCubeSize() const {
    return outputPlanes * outputBoardSize * outputBoardSize;
}
template< typename T > VIRTUAL int InputLayer<T>::getResultsSize() const {
    return batchSize * getOutputCubeSize();
}
template< typename T > VIRTUAL std::string InputLayer<T>::toString() {
    return std::string("") + "InputLayer { outputPlanes " + ::toString( outputPlanes ) + " outputBoardSize " +  ::toString( outputBoardSize ) + " }";
}
template< typename T > VIRTUAL std::string InputLayer<T>::asString() const {
    return std::string("") + "InputLayer { outputPlanes " + ::toString( outputPlanes ) + " outputBoardSize " +  ::toString( outputBoardSize ) + " }";
}

//ostream &operator<<( ostream &os, InputLayer &layer ) {
//    os << "InputLayer { outputPlanes " << layer.outputPlanes << " outputBoardSize " << layer.outputBoardSize << " }";
//    return os;
//}
//ostream &operator<<( ostream &os, InputLayer const*layer ) {
//    os << "InputLayer { outputPlanes " << layer->outputPlanes << " outputBoardSize " << layer->outputBoardSize << " }";
//    return os;
//}


template class InputLayer<float>;
template class InputLayer<unsigned char>;

