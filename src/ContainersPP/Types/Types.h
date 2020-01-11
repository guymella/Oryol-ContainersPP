#pragma once
//------------------------------------------------------------------------------
/**
    @file Core/Types.h
    @brief defines basic data types for Oryol
*/
#include <stdint.h>

namespace ContainersPP {
    namespace Types {

        // those typedefs are obsolete, don't use
        //typedef int8_t int8;
        //typedef uint8_t uint8;
        //typedef int16_t int16;
        //typedef uint16_t uint16;
        //typedef int32_t int32;
        //typedef uint32_t uint32;
        //typedef int64_t int64;
        //typedef uint64_t uint64;
        //typedef float float32;
        //typedef double float64;
        //typedef intptr_t intptr;
        //typedef uintptr_t uintptr;
        //typedef unsigned char uchar;

        static const int32_t InvalidIndex = -1;
        static const int32_t EndOfString = -1;
        static const int32_t EndOfFile = -1;
        static const int32_t EndOfRange = -1;

        bool is_big_endian()
        {
            union {
                uint32_t i;
                char c[4];
            } bint = { 0x01020304 };

            return bint.c[0] == 1;
        }






		static size_t version = 0;
		enum baseTypes : uint8_t {
			Void, //empty uninitialized
			Bit, //1/8 byte
			Byte, // 1 byte
			Short, // 2 bytes
			Word, // 4 bytes 
			Long, // 8 bytes (cpu registers)
			Quad, // 16 bytes (sse register)
			Oct, // 32 bytes (avx register)
			hex, // 64 bytes (avx512 register)
			DoHex, // 128 Bytes (small wave)
			QuadHex, //256 bytes (Wave/cache line)
			OctHex, //512 bytes (large wave/half fetch)
			KiB, //1024 Bytes (small Tensor/ cache fetch) 
			DKiB, //2048 bytes (tensor/wavefront)
			QKiB, // 4096 bytes (Page)
			OKiB, // 8192 bytes (cache Pane)
			HKiB, // 16384 bytes (cache Window)
			WKiB, // 32768 bytes (Cache Bank)
			Bank, // 65536 bytes (ram Bank)
			MiB, // 1048576 Bytes
			GiB, // 1,073,741,824 Bytes
			Fat, // 4,294,967,296? Bytes
			boolean,
			uint8,
			int8,
			uint16,
			int16,
			uint32,
			int32,
			uint64,
			int64,
			uint80,
			int80,
			uint128,
			int128,
			float16,
			float32,
			float64,
			Scalar8_,
			Scalar16_,
			Scalar32_,
			Scalar64_,
			Vector16_,
			Vector32_,
			Vector64_,
			Matrix16_,
			Matrix32_,
			Matrix64_,
			Tensor16_,
			Tensor32_,
			Tensor64_,
			TransLinear,

			decimal,
			chr,
			nchr,

			rational,
			complex,
			quaternion,
			octernion,
			lateral,

			Struct = 100,

			formal = 109, // formal symbolic language
			informal = 110, // informal symbolic language
			Abstract = 111,
			Symbolic = 112,
			Continueouse = 113,
			Disjoint = 114,

			String = 115,
			VarStruct = 116,
			VarBinary = 117,
			VarUint = 118,
			VarInt = 119,
			VarChar = 120,
			VarFloat = 121,
			VarDecimal = 122,
			VarRational = 123,
			VarLateral = 124,

			SRef = 125, //static reference (Domain|key|Hash ::  0|?|? is reserved for standard library,{1-8b}|?|? = local cache Registaration; Hashes smaller than 256 bits are considered version numbers )
			PRef = 126, //phisical reference (ipv6:port|key|Hash)
			LRef = 127, //local reference (1-4B|?|? reserved for process local scopes Catalog, 5-8B|?|?  used for App Catalog 9-12B|?|? for cluster Catalog)

			//BlockS = 128, //Static Block
			//Block = 129,  //Dynamic Block 
			//BlockV = 130, // back loaded table
			//BlockF = 131, //front loaded table
			//BlockD = 132, //Double ended block
			//BlockPwr = 133, //Power allocated Block
			//Allocator = 134, //block Allocator
			//AllocatorPwr = 135, // Power Allocator (Preallocator)
			//TableSF = 136, //Static Table
			//TableS = 137, //Static Fixed table
			//TableF = 138, //dynamic Table fixed Size Partitions
			//TableD = 139, //full dynamic table
			//TableRef = 140, //table of referenced Blocks

			Val = 253, // undefined value stub. polymorphic base value
			Poly = 254 //undefined object stub, pure polymorphic struct 
		};

		static size_t SizeOf(baseTypes type) {
			switch (type)
			{
			case Types::Byte:
				return 1;
			case Types::Short:
				return 2;
			case Types::Word:
				return 4;
			case Types::Long:
				return 8;
			case Types::Quad:
				return 16;
			case Types::Oct:
				return 32;
			case Types::hex:
				return 64;
			case Types::DoHex:
				return 128;
			case Types::QuadHex:
				return 256;
			case Types::OctHex:
				return 512;
			case Types::KiB:
				return 1024;
			case Types::DKiB:
				return 2048;
			case Types::QKiB:
				return 4096;
			case Types::OKiB:
				return 8192;
			case Types::HKiB:
				return 16384;
			case Types::WKiB:
				return 32768;
			case Types::Bank:
				return 65536;
			case Types::MiB:
				return 1048576;
			case Types::GiB:
				return 1073741824;
			case Types::Fat:
				return 4294967296;
			case Types::uint8:
				return sizeof(uint8_t);
			case Types::int8:
				return sizeof(int8_t);
			case Types::uint16:
				return sizeof(uint16_t);
			case Types::int16:
				return sizeof(int16_t);
			case Types::uint32:
				return sizeof(uint32_t);
			case Types::int32:
				return sizeof(int32_t);
			case Types::uint64:
				return sizeof(uint64_t);
			case Types::int64:
				return sizeof(int64_t);
			case Types::uint80:
				return 10;
			case Types::int80:
				return 10;
			case Types::uint128:
				return 16;
			case Types::int128:
				return 16;
			case Types::float16:
				return 2;
			case Types::float32:
				return 4;
			case Types::float64:
				return 8;
			case Types::Scalar8_:
				return 1;
			case Types::Scalar16_:
				return 2;
			case Types::Scalar32_:
				return 4;
			case Types::Scalar64_:
				return 8;
			case Types::Vector16_:
				return 2;
			case Types::Vector32_:
				return 4;
			case Types::Vector64_:
				return 8;
			case Types::Matrix16_:
				return 2;
			case Types::Matrix32_:
				return 4;
			case Types::Matrix64_:
				return 8;
			case Types::Tensor16_:
				return 2;
			case Types::Tensor32_:
				return 4;
			case Types::Tensor64_:
				return 8;
			case Types::TransLinear:
				return 8;
			case Types::decimal:
				return 16;
			case Types::chr:
				return 1;
			case Types::nchr:
				return 4;
			case Types::rational:
				return 16;
			case Types::complex:
				return 16;
			case Types::quaternion:
				return 32;
			case Types::octernion:
				return 64;
			case Types::lateral:
				return 8;

			case Types::Val:
				return 0;
			case Types::Poly:
				return 0;
			case Types::Void:
				return 0;
			default:
				break;
			}
			return 0xffffffffffffffffui64;//unknown size
		}




    }// namespace types
} // namespace ContainersPP

