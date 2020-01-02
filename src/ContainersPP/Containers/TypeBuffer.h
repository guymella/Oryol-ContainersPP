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
    public:
        TypeBlock() {};
        TypeBlock(const TypeBlock& rhs) : buffer(rhs.buffer) {};
        TypeBlock(TypeBlock&& rhs) : buffer(std::move(rhs.buffer)) {};
        ~TypeBlock() { Clear(); };
        /// MoveAssign
        virtual void operator=(TypeBlock&& rhs);
        /// CopyAssign
        virtual void operator=(const TypeBlock& rhs);
    protected:
        virtual iBlockD& Buffer() override;
        virtual const iBlockD& Buffer() const override;
    private:
        Block buffer;
    };

    template <typename TYPE>
    class TypeVector : public iTypeVector<TYPE> {
    public:
        TypeVector() {};
        TypeVector(const TypeVector& rhs) : buffer(rhs.buffer) {};
        TypeVector(TypeVector&& rhs) : buffer(std::move(rhs.buffer)) {};
        ~TypeVector() { Clear(); };
        /// CopyAssign
        virtual void operator=(TypeVector&& rhs);
        /// CopyAssign
        virtual void operator=(const TypeVector& rhs);
    protected:
        virtual iBufferV& Buffer() override;
        virtual const iBufferV& Buffer() const override;
    private:
        ContainersPP::Buffer buffer;
    };

template <typename TYPE>
class TypeBuffer : public iTypeBuffer<TYPE>{
public:
    TypeBuffer() {};
    TypeBuffer(const TypeBuffer& rhs) : buffer(rhs.buffer) {};
    TypeBuffer(TypeBuffer&& rhs) : buffer(std::move(rhs.buffer)) {};
    ~TypeBuffer() { Clear(); };
    /// MoveAssign
    virtual void operator=(TypeBuffer&& rhs);
    /// CopyAssign
    virtual void operator=(const TypeBuffer& rhs);
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
inline void TypeBlock<TYPE>::operator=(TypeBlock&& rhs)
{
    buffer = std::move(rhs.buffer);
}
//template<typename TYPE>
//inline TypeBlock<TYPE>::TypeBlock(TypeBlock&& rhs)
//{
//    buffer = std::move(rhs.Buffer);
//}
template<typename TYPE>
inline void TypeBlock<TYPE>::operator=(const TypeBlock& rhs)
{
    iTypeBlock<TYPE>::operator=(rhs);
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
inline void TypeVector<TYPE>::operator=(TypeVector&& rhs)
{
    buffer = std::move(rhs.buffer);
}
//template<typename TYPE>
//inline TypeVector<TYPE>::TypeVector(TypeVector&& rhs)
//{
//    buffer = std::move(rhs.Buffer);
//}
template<typename TYPE>
inline void TypeVector<TYPE>::operator=(const TypeVector& rhs)
{
    iTypeBlock<TYPE>::operator=(rhs);
}
template<typename TYPE>
inline iBufferV& TypeVector<TYPE>::Buffer() {
    return buffer;
}



//template<typename TYPE>
//inline TypeBuffer<TYPE>::TypeBuffer(TypeBuffer&& rhs)
//{
//    buffer = std::move(rhs.Buffer);
//}

template<typename TYPE>
inline void TypeBuffer<TYPE>::operator=(TypeBuffer&& rhs)
{
    buffer = std::move(rhs.buffer);
}

template<typename TYPE>
inline void TypeBuffer<TYPE>::operator=(const TypeBuffer& rhs)
{
    iTypeBlock<TYPE>::operator=(rhs);
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
