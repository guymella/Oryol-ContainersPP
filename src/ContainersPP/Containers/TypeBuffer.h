#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::BufferDbl
    @ingroup Core
    @brief growable memory buffer for raw data
*/
#include "ContainersPP/Containers/BufferDbl.h"
#include "ContainersPP/Containers/Buffer.h"
#include "ContainersPP/Containers/Block.h"
#include "ContainersPP/Interfaces/iTypeBuffer.h"

namespace ContainersPP {

    template <typename TYPE>
    class TypeBlock : public iTypeBlock<TYPE> {
    protected:
        virtual iBlockD& Buffer() override;
        virtual const iBlockD& Buffer() const override;
    private:
        Block buffer;
    };

    template <typename TYPE>
    class TypeVector : public iTypeVector<TYPE> {
    protected:
        virtual iBufferV& Buffer() override;
        virtual const iBufferV& Buffer() const override;
    private:
        ContainersPP::Buffer buffer;
    };

template <typename TYPE>
class TypeBuffer : public iTypeBuffer<TYPE>{
protected:
    virtual iBufferD& Buffer() override;
    virtual const iBufferD& Buffer() const override;
private:
    BufferDbl buffer;
};




template<typename TYPE>
inline const iBlockD& TypeBlock<TYPE>::Buffer() const {
    return buffer;
}
template<typename TYPE>
inline iBlockD& TypeBlock<TYPE>::Buffer() {
    return buffer;
}



template<typename TYPE>
inline const iBufferV& TypeVector<TYPE>::Buffer() const {
    return buffer;
}
template<typename TYPE>
inline iBufferV& TypeVector<TYPE>::Buffer() {
    return buffer;
}



template<typename TYPE>
inline iBufferD& TypeBuffer<TYPE>::Buffer() {
    return buffer;
}
template<typename TYPE>
inline const iBufferD& TypeBuffer<TYPE>::Buffer() const {
    return buffer;
}

} // namespace 
