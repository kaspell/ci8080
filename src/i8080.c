#include <stdio.h>
#include <stdlib.h>

#include "i8080.h"


/* -------------------------------------------------------------------------- |
 |                                                                            |
 |                            INSTRUCTION OPCODES                             |
 |                                                                            |
 | -------------------------------------------------------------------------- */

/*** CARRY INSTRUCTIONS ***/

#define STC 0x37  /* STC (Set carry). The Carry bit is set to 1. */
#define CMC 0x3F  /* CMC (Complement carry). If the Carry bit == 0, it is set to 1. If the carry bit == 1 it is reset to 0. */


/*** SINGLE REGISTER INSTRUCTIONS ***/

#define INR_B 0x04  /* INR B (Increment register or memory). The register B is incremented by one. */
#define INR_C 0x0C  /* INR C (Increment register or memory). The register C is incremented by one. */
#define INR_D 0x14  /* INR D (Increment register or memory). The register D is incremented by one. */
#define INR_E 0x1C  /* INR E (Increment register or memory). The register E is incremented by one. */
#define INR_H 0x24  /* INR H (Increment register or memory). The register H is incremented by one. */
#define INR_L 0x2C  /* INR L (Increment register or memory). The register L is incremented by one. */
#define INR_M 0x34  /* INR M (Increment register or memory). The memory location addressed by the register pair HL is incremented by one. */
#define INR_A 0x3C  /* INR A (Increment register or memory). The accumulator register A is incremented by one. */

#define DCR_B 0x05  /* DCR (Decrement register or memory). The register B is decremented by one. */
#define DCR_C 0x0D  /* DCR (Decrement register or memory). The register C is decremented by one. */
#define DCR_D 0x15  /* DCR (Decrement register or memory). The register D is decremented by one. */
#define DCR_E 0x1D  /* DCR (Decrement register or memory). The register E is decremented by one. */
#define DCR_H 0x25  /* DCR (Decrement register or memory). The register H is decremented by one. */
#define DCR_L 0x2D  /* DCR (Decrement register or memory). The register L is decremented by one. */
#define DCR_M 0x35  /* DCR (Decrement register or memory). The memory location addressed by the register pair HL is decremented by one. */
#define DCR_A 0x3D  /* DCR (Decrement register or memory). The accumulator register A is decremented by one. */

#define CMA 0x2F  /* CMA (Complement accumulator). Each bit of the contents of the accumulator register is complemented (producing the one's complement). */

#define DAA 0x27  /* The eight-bit hexadecimal number held in the accumulator register A is adjusted to form two four-bit binary-coded decimal digits by the following two step process: (1) If the least significant four bits of the accumulator represents a number greater than 9, or if the Auxiliary Carry bit is equal to one, the accumulator A is incremented by six. Otherwise, no incrementing occurs. (2) If the most significant four bits of the accumulator A now represent a number greater than 9, or if the normal carry bit is equal to one, the most significant four bits of the accumulator are incremented by six. Otherwise, no incrementing occurs. */


/*** NOP INSTRUCTION ***/

#define NOP 0x00  /* NOP (No operation). No operation occurs. Execution proceeds with the next sequential instruction. */


/*** DATA TRANSFER INSTRUCTIONS ***/

#define MOV_B_B 0x40  /* MOV B B (Move). One byte of data is moved from the register B to the register B. */
#define MOV_B_C 0x41  /* MOV B C (Move). One byte of data is moved from the register C to the register B. */
#define MOV_B_D 0x42  /* MOV B D (Move). One byte of data is moved from the register D to the register B. */
#define MOV_B_E 0x43  /* MOV B E (Move). One byte of data is moved from the register E to the register B. */
#define MOV_B_H 0x44  /* MOV B H (Move). One byte of data is moved from the register H to the register B. */
#define MOV_B_L 0x45  /* MOV B L (Move). One byte of data is moved from the register L to the register B. */
#define MOV_B_M 0x46  /* MOV B M (Move). One byte of data is moved from the memory location addressed by the register pair HL to the register B. */
#define MOV_B_A 0x47  /* MOV B A (Move). One byte of data is moved from the accumulator register A to the register B. */

#define MOV_C_B 0x48  /* MOV C B (Move). One byte of data is moved from the register B to the register C. */
#define MOV_C_C 0x49  /* MOV C C (Move). One byte of data is moved from the register C to the register C. */
#define MOV_C_D 0x4A  /* MOV C D (Move). One byte of data is moved from the register D to the register C. */
#define MOV_C_E 0x4B  /* MOV C E (Move). One byte of data is moved from the register E to the register C. */
#define MOV_C_H 0x4C  /* MOV C H (Move). One byte of data is moved from the register H to the register C. */
#define MOV_C_L 0x4D  /* MOV C L (Move). One byte of data is moved from the register L to the register C. */
#define MOV_C_M 0x4E  /* MOV C M (Move). One byte of data is moved from the memory location addressed by the register pair HL to the register C. */
#define MOV_C_A 0x4F  /* MOV C A (Move). One byte of data is moved from the accumulator register A to the register C. */

#define MOV_D_B 0x50  /* MOV D B (Move). One byte of data is moved from the register B to the register D. */
#define MOV_D_C 0x51  /* MOV D C (Move). One byte of data is moved from the register C to the register D. */
#define MOV_D_D 0x52  /* MOV D D (Move). One byte of data is moved from the register D to the register D. */
#define MOV_D_E 0x53  /* MOV D E (Move). One byte of data is moved from the register E to the register D. */
#define MOV_D_H 0x54  /* MOV D H (Move). One byte of data is moved from the register H to the register D. */
#define MOV_D_L 0x55  /* MOV D L (Move). One byte of data is moved from the register L to the register D. */
#define MOV_D_M 0x56  /* MOV D M (Move). One byte of data is moved from the memory location addressed by the register pair HL to the register D. */
#define MOV_D_A 0x57  /* MOV D A (Move). One byte of data is moved from the accumulator register A to the register D. */

#define MOV_E_B 0x58  /* MOV E B (Move). One byte of data is moved from the register B to the register E. */
#define MOV_E_C 0x59  /* MOV E C (Move). One byte of data is moved from the register C to the register E. */
#define MOV_E_D 0x5A  /* MOV E D (Move). One byte of data is moved from the register D to the register E. */
#define MOV_E_E 0x5B  /* MOV E E (Move). One byte of data is moved from the register E to the register E. */
#define MOV_E_H 0x5C  /* MOV E H (Move). One byte of data is moved from the register H to the register E. */
#define MOV_E_L 0x5D  /* MOV E L (Move). One byte of data is moved from the register L to the register E. */
#define MOV_E_M 0x5E  /* MOV E M (Move). One byte of data is moved from the memory location addressed by the register pair HL to the register E. */
#define MOV_E_A 0x5F  /* MOV E A (Move). One byte of data is moved from the accumulator register A to the register E. */

#define MOV_H_B 0x60  /* MOV H B (Move). One byte of data is moved from the register B to the register H. */
#define MOV_H_C 0x61  /* MOV H C (Move). One byte of data is moved from the register C to the register H. */
#define MOV_H_D 0x62  /* MOV H D (Move). One byte of data is moved from the register D to the register H. */
#define MOV_H_E 0x63  /* MOV H E (Move). One byte of data is moved from the register E to the register H. */
#define MOV_H_H 0x64  /* MOV H H (Move). One byte of data is moved from the register H to the register H. */
#define MOV_H_L 0x65  /* MOV H L (Move). One byte of data is moved from the register L to the register H. */
#define MOV_H_M 0x66  /* MOV H M (Move). One byte of data is moved from the memory location addressed by the register pair HL to the register H. */
#define MOV_H_A 0x67  /* MOV H A (Move). One byte of data is moved from the accumulator register A to the register H. */

#define MOV_L_B 0x68  /* MOV L B (Move). One byte of data is moved from the register B to the register L. */
#define MOV_L_C 0x69  /* MOV L C (Move). One byte of data is moved from the register C to the register L. */
#define MOV_L_D 0x6A  /* MOV L D (Move). One byte of data is moved from the register D to the register L. */
#define MOV_L_E 0x6B  /* MOV L E (Move). One byte of data is moved from the register E to the register L. */
#define MOV_L_H 0x6C  /* MOV L H (Move). One byte of data is moved from the register H to the register L. */
#define MOV_L_L 0x6D  /* MOV L L (Move). One byte of data is moved from the register L to the register L. */
#define MOV_L_M 0x6E  /* MOV L M (Move). One byte of data is moved from the memory location addressed by the register pair HL to the register L. */
#define MOV_L_A 0x6F  /* MOV L A (Move). One byte of data is moved from the accumulator register A to the register L. */

#define MOV_M_B 0x70  /* MOV M B (Move). One byte of data is moved from the register B to the memory location addressed by the register pair HL. */
#define MOV_M_C 0x71  /* MOV M C (Move). One byte of data is moved from the register C to the memory location addressed by the register pair HL. */
#define MOV_M_D 0x72  /* MOV M D (Move). One byte of data is moved from the register D to the memory location addressed by the register pair HL. */
#define MOV_M_E 0x73  /* MOV M E (Move). One byte of data is moved from the register E to the memory location addressed by the register pair HL. */
#define MOV_M_H 0x74  /* MOV M H (Move). One byte of data is moved from the register H to the memory location addressed by the register pair HL. */
#define MOV_M_L 0x75  /* MOV M L (Move). One byte of data is moved from the register L to the memory location addressed by the register pair HL. */
// The MOV_M_M instruction is intentionally not present since it would be a no-op and since its opcode would coincide with the opcode of the HLT instruction
#define MOV_M_A 0x77  /* MOV M A (Move). One byte of data is moved from the accumulator register A to the register M. */

#define MOV_A_B 0x78  /* MOV A B (Move). One byte of data is moved from the register B to the accumulator register A. */
#define MOV_A_C 0x79  /* MOV A C (Move). One byte of data is moved from the register C to the accumulator register A. */
#define MOV_A_D 0x7A  /* MOV A D (Move). One byte of data is moved from the register D to the accumulator register A. */
#define MOV_A_E 0x7B  /* MOV A E (Move). One byte of data is moved from the register E to the accumulator register A. */
#define MOV_A_H 0x7C  /* MOV A H (Move). One byte of data is moved from the register H to the accumulator register A. */
#define MOV_A_L 0x7D  /* MOV A L (Move). One byte of data is moved from the register L to the accumulator register A. */
#define MOV_A_M 0x7E  /* MOV A M (Move). One byte of data is moved from the memory location addressed by the register pair HL to the accumulator register A. */
#define MOV_A_A 0x7F  /* MOV A A (Move). One byte of data is moved from the register A to the accumulator register A. */

#define STAX_BC 0x02  /* STAX B (Store accumulator). The contents of the accumulator register are stored in the memory location addressed by the register pair BC. */
#define STAX_DE 0x12  /* STAX D (Store accumulator). The contents of the accumulator register are stored in the memory location addressed by the register pair DE. */

#define LDAX_BC 0x0A  /* LDAX B (Load accumulator). The contents of the memory location addressed by register pair BC replace the contents of the accumulator register. */
#define LDAX_DE 0x1A  /* LDAX B (Load accumulator). The contents of the memory location addressed by register pair DE replace the contents of the accumulator register. */


/*** REGISTER OR MEMORY TO ACCUMULATOR INSTRUCTIONS ***/

#define ADD_B 0x80  /* ADD B (Add register or memory to accumulator). The byte held in the register B is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADD_C 0x81  /* ADD C (Add register or memory to accumulator). The byte held in the register C is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADD_D 0x82  /* ADD D (Add register or memory to accumulator). The byte held in the register D is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADD_E 0x83  /* ADD E (Add register or memory to accumulator). The byte held in the register E is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADD_H 0x84  /* ADD H (Add register or memory to accumulator). The byte held in the register H is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADD_L 0x85  /* ADD L (Add register or memory to accumulator). The byte held in the register L is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADD_M 0x86  /* ADD M (Add register or memory to accumulator). The byte held in the memory location addressed by the register pair HL is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADD_A 0x87  /* ADD A (Add register or memory to accumulator). The byte held in the accumulator register A is added to the contents of the accumulator register A using two's complement arithmetic. */

#define ADC_B 0x88  /* ADC B (Add register or memory to accumulator with carry). The byte held in the register B plus the content of the Carry bit is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADC_C 0x89  /* ADC C (Add register or memory to accumulator with carry). The byte held in the register C plus the content of the Carry bit is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADC_D 0x8A  /* ADC D (Add register or memory to accumulator with carry). The byte held in the register D plus the content of the Carry bit is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADC_E 0x8B  /* ADC E (Add register or memory to accumulator with carry). The byte held in the register E plus the content of the Carry bit is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADC_H 0x8C  /* ADC H (Add register or memory to accumulator with carry). The byte held in the register H plus the content of the Carry bit is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADC_L 0x8D  /* ADC L (Add register or memory to accumulator with carry). The byte held in the register L plus the content of the Carry bit is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADC_M 0x8E  /* ADC M (Add register or memory to accumulator with carry). The byte held in the register M plus the content of the Carry bit is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ADC_A 0x8F  /* ADC A (Add register or memory to accumulator with carry). The byte held in the register A plus the content of the Carry bit is added to the contents of the accumulator register A using two's complement arithmetic. */

#define SUB_B 0x90  /* SUB B (Subtract register or memory from accumulator). The byte held in the register B is subtracted from the contents of the accumulator register A using two's complement arithmetic. */
#define SUB_C 0x91  /* SUB C (Subtract register or memory from accumulator). The byte held in the register C is subtracted from the contents of the accumulator register A using two's complement arithmetic. */
#define SUB_D 0x92  /* SUB D (Subtract register or memory from accumulator). The byte held in the register D is subtracted from the contents of the accumulator register A using two's complement arithmetic. */
#define SUB_E 0x93  /* SUB E (Subtract register or memory from accumulator). The byte held in the register E is subtracted from the contents of the accumulator register A using two's complement arithmetic. */
#define SUB_H 0x94  /* SUB H (Subtract register or memory from accumulator). The byte held in the register H is subtracted from the contents of the accumulator register A using two's complement arithmetic. */
#define SUB_L 0x95  /* SUB L (Subtract register or memory from accumulator). The byte held in the register L is subtracted from the contents of the accumulator register A using two's complement arithmetic. */
#define SUB_M 0x96  /* SUB M (Subtract register or memory from accumulator). The byte held in the memory location addressed by the register pair HL is subtracted from the contents of the accumulator register A using two's complement arithmetic. */
#define SUB_A 0x97  /* SUB A (Subtract register or memory from accumulator). The byte held in the accumulator register A is subtracted from the contents of the accumulator register A using two's complement arithmetic. */

#define SBB_B 0x98  /* SBB B (Subtract register or memory from accumulator with borrow). The Carry bit is added to the contents of the register B. The value is then subtracted from the accumulator register A using two's complement arithmetic. */
#define SBB_C 0x99  /* SBB C (Subtract register or memory from accumulator with borrow). The Carry bit is added to the contents of the register C. The value is then subtracted from the accumulator register A using two's complement arithmetic. */
#define SBB_D 0x9A  /* SBB D (Subtract register or memory from accumulator with borrow). The Carry bit is added to the contents of the register D. The value is then subtracted from the accumulator register A using two's complement arithmetic. */
#define SBB_E 0x9B  /* SBB E (Subtract register or memory from accumulator with borrow). The Carry bit is added to the contents of the register E. The value is then subtracted from the accumulator register A using two's complement arithmetic. */
#define SBB_H 0x9C  /* SBB H (Subtract register or memory from accumulator with borrow). The Carry bit is added to the contents of the register H. The value is then subtracted from the accumulator register A using two's complement arithmetic. */
#define SBB_L 0x9D  /* SBB L (Subtract register or memory from accumulator with borrow). The Carry bit is added to the contents of the register L. The value is then subtracted from the accumulator register A using two's complement arithmetic. */
#define SBB_M 0x9E  /* SBB M (Subtract register or memory from accumulator with borrow). The Carry bit is added to the byte held in the memory location addressed by the register pair HL. The value is then subtracted from the accumulator register A using two's complement arithmetic. */
#define SBB_A 0x9F  /* SBB A (Subtract register or memory from accumulator with borrow). The Carry bit is added to the contents of the accumulator register A. The value is then subtracted from the accumulator register A using two's complement arithmetic. */

#define ANA_B 0xA0  /* ANA B (Logical AND register or memory with accumulator). The byte held in the register B is logically ANDed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ANA_C 0xA1  /* ANA C (Logical AND register or memory with accumulator). The byte held in the register C is logically ANDed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ANA_D 0xA2  /* ANA D (Logical AND register or memory with accumulator). The byte held in the register D is logically ANDed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ANA_E 0xA3  /* ANA E (Logical AND register or memory with accumulator). The byte held in the register E is logically ANDed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ANA_H 0xA4  /* ANA H (Logical AND register or memory with accumulator). The byte held in the register H is logically ANDed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ANA_L 0xA5  /* ANA L (Logical AND register or memory with accumulator). The byte held in the register L is logically ANDed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ANA_M 0xA6  /* ANA M (Logical AND register or memory with accumulator). The byte held in the memory location addressed by the register pair HL is logically ANDed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ANA_A 0xA7  /* ANA A (Logical AND register or memory with accumulator). The byte held in the accumulator register A is logically ANDed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */

#define XRA_B 0xA8  /* XRA B (Logical XOR register or memory with accumulator). The byte held in the register B is XORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define XRA_C 0xA9  /* XRA C (Logical XOR register or memory with accumulator). The byte held in the register C is XORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define XRA_D 0xAA  /* XRA D (Logical XOR register or memory with accumulator). The byte held in the register D is XORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define XRA_E 0xAB  /* XRA E (Logical XOR register or memory with accumulator). The byte held in the register E is XORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define XRA_H 0xAC  /* XRA H (Logical XOR register or memory with accumulator). The byte held in the register H is XORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define XRA_L 0xAD  /* XRA L (Logical XOR register or memory with accumulator). The byte held in the register L is XORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define XRA_M 0xAE  /* XRA M (Logical XOR register or memory with accumulator). The byte held in the memory location addressed by the register pair HL is XORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define XRA_A 0xAF  /* XRA A (Logical XOR register or memory with accumulator). The byte held in the register A is XORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */

#define ORA_B 0xB0  /* ORA B (Logical OR register or memory with accumulator). The byte held in the register B is ORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ORA_C 0xB1  /* ORA C (Logical OR register or memory with accumulator). The byte held in the register C is ORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ORA_D 0xB2  /* ORA D (Logical OR register or memory with accumulator). The byte held in the register D is ORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ORA_E 0xB3  /* ORA E (Logical OR register or memory with accumulator). The byte held in the register E is ORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ORA_H 0xB4  /* ORA H (Logical OR register or memory with accumulator). The byte held in the register H is ORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ORA_L 0xB5  /* ORA L (Logical OR register or memory with accumulator). The byte held in the register L is ORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ORA_M 0xB6  /* ORA M (Logical OR register or memory with accumulator). The byte held in the memory location addressed by the register pair HL is ORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define ORA_A 0xB7  /* ORA A (Logical OR register or memory with accumulator). The byte held in the register A is ORed bit by bit with the contents of the accumulator register A. The Carry bit is reset to zero. */

#define CMP_B 0xB8  /* CMP B (Compare register or memory with accumulator). The byte held in the register B is compared to the contents of the accumulator. The comparison is performed by internally subtracting the contents of the register from the accumulator (leaving both unchanged) and setting the conditions bits according to the result. In particular, the Zero bit is set if the quantities are equal, and reset if they are unequal. Since a subtraction operation is performed, the Carry bit will be set if there is no carry out of bit 7, indicating that the contents of B are greater than the contents of the accumulator, and reset otherwise. */
#define CMP_C 0xB9  /* CMP C (Compare register or memory with accumulator). The byte held in the register C is compared to the contents of the accumulator. The comparison is performed by internally subtracting the contents of the register from the accumulator (leaving both unchanged) and setting the conditions bits according to the result. In particular, the Zero bit is set if the quantities are equal, and reset if they are unequal. Since a subtraction operation is performed, the Carry bit will be set if there is no carry out of bit 7, indicating that the contents of C are greater than the contents of the accumulator, and reset otherwise. */
#define CMP_D 0xBA  /* CMP D (Compare register or memory with accumulator). The byte held in the register D is compared to the contents of the accumulator. The comparison is performed by internally subtracting the contents of the register from the accumulator (leaving both unchanged) and setting the conditions bits according to the result. In particular, the Zero bit is set if the quantities are equal, and reset if they are unequal. Since a subtraction operation is performed, the Carry bit will be set if there is no carry out of bit 7, indicating that the contents of D are greater than the contents of the accumulator, and reset otherwise. */
#define CMP_E 0xBB  /* CMP E (Compare register or memory with accumulator). The byte held in the register E is compared to the contents of the accumulator. The comparison is performed by internally subtracting the contents of the register from the accumulator (leaving both unchanged) and setting the conditions bits according to the result. In particular, the Zero bit is set if the quantities are equal, and reset if they are unequal. Since a subtraction operation is performed, the Carry bit will be set if there is no carry out of bit 7, indicating that the contents of E are greater than the contents of the accumulator, and reset otherwise. */
#define CMP_H 0xBC  /* CMP H (Compare register or memory with accumulator). The byte held in the register H is compared to the contents of the accumulator. The comparison is performed by internally subtracting the contents of the register from the accumulator (leaving both unchanged) and setting the conditions bits according to the result. In particular, the Zero bit is set if the quantities are equal, and reset if they are unequal. Since a subtraction operation is performed, the Carry bit will be set if there is no carry out of bit 7, indicating that the contents of H are greater than the contents of the accumulator, and reset otherwise. */
#define CMP_L 0xBD  /* CMP L (Compare register or memory with accumulator). The byte held in the register L is compared to the contents of the accumulator. The comparison is performed by internally subtracting the contents of the register from the accumulator (leaving both unchanged) and setting the conditions bits according to the result. In particular, the Zero bit is set if the quantities are equal, and reset if they are unequal. Since a subtraction operation is performed, the Carry bit will be set if there is no carry out of bit 7, indicating that the contents of L are greater than the contents of the accumulator, and reset otherwise. */
#define CMP_M 0xBE  /* CMP M (Compare register or memory with accumulator). The byte held in the memory location addressed by the register pair HL is compared to the contents of the accumulator. The comparison is performed by internally subtracting the contents of the memory location from the accumulator (leaving both unchanged) and setting the conditions bits according to the result. In particular, the Zero bit is set if the quantities are equal, and reset if they are unequal. Since a subtraction operation is performed, the Carry bit will be set if there is no carry out of bit 7, indicating that the contents of the memory location are greater than the contents of the accumulator, and reset otherwise. */
#define CMP_A 0xBF  /* CMP A (Compare register or memory with accumulator). The byte held in the accumulator register A is compared to the contents of the accumulator. The comparison is performed by internally subtracting the contents of the register from the accumulator (leaving both unchanged) and setting the conditions bits according to the result. In particular, the Zero bit is set if the quantities are equal, and reset if they are unequal. Since a subtraction operation is performed, the Carry bit will be set if there is no carry out of bit 7, indicating that the contents of A are greater than the contents of the accumulator, and reset otherwise. */


/*** ROTATE ACCUMULATOR INSTRUCTIONS ***/

#define RLC 0x07  /* RLC (Rotate accumulator left). The Carry bit is set equal to the high-order bit of the accumulator register A. The contents of the accumulator are rotated one bit position to the left, with the high-order bit being transferred to the low-order bit position of the accumulator. */
#define RRC 0x0F  /* RRC (Rotate accumulator right). The Carry bit is set equal to the low-order bit of the accumulator register A. The contents of the accumulator are ro */
#define RAL 0x17  /* RAL (Rotate accumulator left through carry). The contents of the accumulator are rotated one bit position to the left. */
#define RAR 0x1F  /* RAR (Rotate accumulator right through carry). The contents of the accumulator are rotated one bit position to the right. */


/*** REGISTER PAIR INSTRUCTIONS ***/

#define PUSH_BC 0xC5  /* PUSH BC (Push data onto stack). The contents of the register pair BC are saved in two bytes of memory indicated by the stack pointer SP. */
#define PUSH_DE 0xD5  /* PUSH DE (Push data onto stack). The contents of the register pair DE are saved in two bytes of memory indicated by the stack pointer SP. */
#define PUSH_HL 0xE5  /* PUSH HL (Push data onto stack). The contents of the register pair HL are saved in two bytes of memory indicated by the stack pointer SP. */
#define PUSH_PSW 0xF5  /* PUSH PSW (Push data onto stack). The contents of the Program Status Word (PSW; the 16-bit number where the high-order byte is saved in the accumulator register A and where the low-order byte is saved in the flag byte) are saved in two bytes of memory indicated by the stack pointer SP. */

#define POP_BC 0xC1  /* POP BC (Pop data off stack). The contents of the register pair BC are restored from two bytes of memory indicated by the stack pointer SP. */
#define POP_DE 0xD1  /* POP DE (Pop data off stack). The contents of the register pair DE are restored from two bytes of memory indicated by the stack pointer SP. */
#define POP_HL 0xE1  /* POP HL (Pop data off stack). The contents of the register pair HL are restored from two bytes of memory indicated by the stack pointer SP. */
#define POP_PSW 0xF1  /* POP PSW (Pop data off stack). The contents of the Program Status Word (PSW; the 16-bit number where the high-order byte is saved in the accumulator register A and where the low-order byte is saved in the flag byte) are restored from two bytes of memory indicated by the stack pointer SP. */

#define DAD_BC 0x09  /* DAD BC (Double add). The 16-bit number in the register pair BC is added to the 16-bit number held in the register pair HL using two's complement arithmetic. */
#define DAD_DE 0x19  /* DAD DE (Double add). The 16-bit number in the register pair DE is added to the 16-bit number held in the register pair HL using two's complement arithmetic. */
#define DAD_HL 0x29  /* DAD HL (Double add). The 16-bit number in the register pair HL is added to the 16-bit number held in the register pair HL using two's complement arithmetic. */
#define DAD_SP 0x39  /* DAD SP (Double add). The 16-bit number in the stack pointer SP is added to the 16-bit number held in the register pair HL using two's complement arithmetic. */

#define INX_BC 0x03  /* INX BC (Increment register pair). The 16-bit number held in the register pair BC is incremented by one. */
#define INX_DE 0x13  /* INX DE (Increment register pair). The 16-bit number held in the register pair DE is incremented by one. */
#define INX_HL 0x23  /* INX HL (Increment register pair). The 16-bit number held in the register pair HL is incremented by one. */
#define INX_SP 0x33  /* INX SP (Increment register pair). The 16-bit number held in the stack pointer is incremented by one. */

#define DCX_BC 0x0B  /* DCX BC (Decrement register pair). The 16-bit number held in the register pair BC is decremented by one. */
#define DCX_DE 0x1B  /* DCX DE (Decrement register pair). The 16-bit number held in the register pair DE is decremented by one. */
#define DCX_HL 0x2B  /* DCX HL (Decrement register pair). The 16-bit number held in the register pair HL is decremented by one. */
#define DCX_SP 0x3B  /* DCX SP (Decrement register pair). The 16-bit number held in the stack pointer is decremented by one. */

#define XCHG 0xEB  /* XCHG (Exchange registers). The 16 bits of data held in the register pair HL is exchanged with the 16 bits of data held in the register pair DE. */
#define XTHL 0xE3  /* XTHL (Exchange stack). The contents of the L register are exchanged with the contents of the memory byte whose address is held in the stack pointer SP. The contents of the H register are exchanged with the contents of the memory byte whose address is onoe greater than that held in the stack pointer SP. */
#define SPHL 0xF9  /* SPHL (Load SP from H and L). The 16 bits of data held in the register pair HL replaces the contents of the stack pointer SP. The register pair HL is left unchanged. */


/*** IMMEDIATE INSTRUCTIONS ***/

#define LXI 0x01

#define MVI_B 0x06  /* MVI B (Move Immediate Data). The byte of immediate data is stored in the register B. */
#define MVI_C 0x0E  /* MVI C (Move Immediate Data). The byte of immediate data is stored in the register C. */
#define MVI_D 0x16  /* MVI D (Move Immediate Data). The byte of immediate data is stored in the register D. */
#define MVI_E 0x1E  /* MVI E (Move Immediate Data). The byte of immediate data is stored in the register E. */
#define MVI_H 0x26  /* MVI H (Move Immediate Data). The byte of immediate data is stored in the register H. */
#define MVI_L 0x2E  /* MVI L (Move Immediate Data). The byte of immediate data is stored in the register L. */
#define MVI_M 0x36  /* MVI M (Move Immediate Data). */
#define MVI_A 0x3E  /* MVI A (Move Immediate Data). The byte of immediate data is stored in the accumulator register A. */

#define ADI 0xC6  /* ADI (Add immediate to accumulator). The byte of immediate data is added to the contents of the accumulator register A using two's complement arithmetic. */
#define ACI 0xCE  /* ACI (Add immediate to accumulator with carry). The byte of immediate data pus the contents of the Carry bit are added to the contents of the accumulator register A. */
#define SUI 0xD6  /* SUI (Subtract immediate from accumulator). The byte of immediate data is subtracted from the contents of the accumulator register A using two's complement arithmetic. */
#define SBI 0xDE  /* SBI (Subtract immediate from accumulator with borrow). The Carry bit is internally added to the byte of immediate data. This value is then subtracted from the accumulator register A using two's complement arithmetic. */
#define ANI 0xE6  /* ANI (AND immediate with accumulator). The byte of immediate data is logically ANDed with the contents of the accumulator register A. The Carry bit is reset to zero. */
#define XRI 0xEE  /* XRI (XOR immediate with accumulator). The byte of immediate data is EXCLUSIVE-ORed with the contents of the accumulator register A. The Carry bit is set to zero. */
#define ORI 0xF6  /* ORI (OR immediate with accumulator). The byte of immediate data is logically ORed with the contents of the accumulator register A. */
#define CPI 0xFE  /* CPI (Compare immediate with accumulator). The byte of immediate data is compared to the contents of the accumulator register A. The comparison is performed by internally subtracting the data from the accumulator using two's complement arithmetic, leaving the accumulator unchanged byt setting the condition bits by the result. In particular, the zero bit is set if the quantities are equal, and reset if they are unequal. */


/*** DIRECT ADDRESSING INSTRUCTIONS ***/

#define STA 0x32  /* STA (Store accumulator direct). The contents of the accumulator register A replace the byte at the memory address formed by concatenating the two bytes of immediate data. */
#define LDA 0x3A  /* LDA (Load accumulator direct). The byte at the memory address formed by concatenating the two bytes of immediate data replaces the contents of the accumulator register A. */
#define SHLD 0x22  /* SHLD (Store H and L direct). The contents of the L register are stored at the memory address formed by concatenating the two bytes of immediate data. The contents of the H register are stored at the next higher memory address. */
#define LHLD 0x2A  /* LHLD (Load H and L direct). The byte at the memory address formed by concatenating the two bytes of immediate data replaces the contents of the L register. The byte at the next higher memory address replaces the contents of the H register. */


/*** JUMP INSTRUCTIONS ***/

#define PCHL 0xE9  /* PCHL (Load Program Counter). The contents of the H register replace the most significant 8 bits of the program counter, and the contents of the L regoster replace the least significant 8 bits of the program counter. This causes program execution to continue at the address contained in the register pair HL. */

#define JMP 0xC3  /* JMP (Jump). Program execution continues unconditionally at the memory address formed by concatenating the two bytes of immediate data, with the first byte. */
#define JC 0xDA  /* JC (Jump if carry). If the Carry bit is one, program execution continues at the memory address formed by concatenating the two bytes of immediate data. */
#define JNC 0xD2  /* JNC (Jump if no carry). If the Carry bit is zero, program execution continues at the memory address formed by concatenating the two bytes of immediate data. */
#define JZ 0xCA  /* JZ (Jump if zero). If the Zero bit is one, program execution continues at the memory address formed by concatenating the two bytes of immediate data. */
#define JNZ 0xC2  /* JNZ (Jump if not zero). If the Zero bit is zero, program execution continues at the memory address formed by concatenating the two bytes of immediate data. */
#define JM 0xFA  /* JM (Jump if minus). If the Sign bit is one (indicating a negative result), program execution continues at the memory address formed by concatenating the two bytes of immediate data. */
#define JP 0xF2  /* JP (Jump if positive). If the Sign bit is zero (indicating a positive result), program execution continues at the memory address formed by concatenating the two bytes of immediate data. */
#define JPE 0xEA  /* JPE (Jump if parity even). If the Parity bit is one (indicating a result with even parity), program execution continues at the memory address formed by concatenating the two bytes of immediate data. */
#define JPO 0xE2  /* JPO (Jump if parity odd). If the Parity bit is zero (indicating a result with odd parity), program execution continues at the memory address formed by concatenating the two bytes of immediate data. */


/*** CALL SUBROUTINE INSTRUCTIONS ***/

#define CALL 0xCD  /* CALL (Call). A call operation is unconditionally performed to a subroutine stored at the memory address formed by concatenating the two bytes of immediate data. */
#define CC 0xDC  /* CC (Call if carry). If the Carry bit is one, a call operation is performed to a subroutine stored at the memory address formed by concatenating the two bytes of immediate data. */
#define CNC 0xD4  /* CNC (Call if not carry). If the Carry bit is zero, a call operation is performed to a subroutine stored at the memory address formed by concatenating the two bytes of immediate data. */
#define CZ 0xCC  /* CZ (Call if zero). If the Zero bit is zero, a call operation is performed to a subroutine stored at the memory address formed by concatenating the two bytes of immediate data. */
#define CNZ 0xC4  /* CNZ (Call if not zero). If the Zero bit is one, a call operation is performed to a subroutine stored at the memory address formed by concatenating the two bytes of immediate data. */
#define CM 0xFC  /* CM (Call if minus). If the Sign bit is one (indicating a minus result), a call operation is performed to a subroutine stored at the memory address formed by concatenating the two bytes of immediate data. */
#define CP 0xF4  /* CP (Call if positive). If the Sign bit is zero (indicating a positive result), a call operation is performed to a subroutine stored at the memory address formed by concatenating the two bytes of immediate data. */
#define CPE 0xEC  /* CPE (Call if parity even). If the Parity bit is one (indicating even parity), a call operation is performed to a subroutine stored at the memory address formed by concatenating the two bytes of immediate data. */
#define CPO 0xE4  /* CPO (Call if parity odd). If the Parity bit is zero (indicating odd parity), a call operation is performed to a subroutine stored at the memory address formed by concatenating the two bytes of immediate data. */


/*** RETURN FROM SUBROUTINE INSTRUCTIONS ***/

#define RET 0xC9  /* RET (Return). A return operation is unconditionally performed. */
#define RC 0xD8  /* RC (Return if carry). If the Carry bit is one, a return operation is performed. */
#define RNC 0xD0  /* RNC (Return if no carry). If the Carry bit is zero, a return operation is performed */
#define RZ 0xC8  /* RZ (Return if zero). If the Zero bit is one, a return operation is performed. */
#define RNZ 0xC0  /* RNZ (Return if not zero). If the Zero bit is zero, a return operation is performed. */
#define RM 0xF8  /* RM (Return if minus). If the Sign bit is one (indicating a minus result), a return operation is performed. */
#define RP 0xF0  /* RP (Return if positive). If the Sign bit is zero (indicating a positive result), a return operation is performed. */
#define RPE 0xE8  /* RPE (Return if parity even). If the Parity bit is one (indicating even parity), a return operation is performed. */
#define RPO 0xE0  /* RPO (Return if parity odd). If the Parity bit is zero (indicating odd parity), a return operation is performed. */


/*** RST INSTRUCTION ***/

#define RST_000 0xC7  /* RST 000 (Restart). The contents of the program counter PC are pushed onto the stack, providing a return address for later use by a RETURN instruction. Program execution continues at the memory address 0000 0000 00XX X000 where XXX is the suffix of the instruction, a number in the range 000 to 111 (in base 2). */
#define RST_001 0xCF  /* RST 001 (Restart). The contents of the program counter PC are pushed onto the stack, providing a return address for later use by a RETURN instruction. Program execution continues at the memory address 0000 0000 00XX X000 where XXX is the suffix of the instruction, a number in the range 000 to 111 (in base 2). */
#define RST_010 0xD7  /* RST 010 (Restart). The contents of the program counter PC are pushed onto the stack, providing a return address for later use by a RETURN instruction. Program execution continues at the memory address 0000 0000 00XX X000 where XXX is the suffix of the instruction, a number in the range 000 to 111 (in base 2). */
#define RST_100 0xE7  /* RST 100 (Restart). The contents of the program counter PC are pushed onto the stack, providing a return address for later use by a RETURN instruction. Program execution continues at the memory address 0000 0000 00XX X000 where XXX is the suffix of the instruction, a number in the range 000 to 111 (in base 2). */
#define RST_101 0xEF  /* RST 101 (Restart). The contents of the program counter PC are pushed onto the stack, providing a return address for later use by a RETURN instruction. Program execution continues at the memory address 0000 0000 00XX X000 where XXX is the suffix of the instruction, a number in the range 000 to 111 (in base 2). */
#define RST_110 0xF7  /* RST 110 (Restart). The contents of the program counter PC are pushed onto the stack, providing a return address for later use by a RETURN instruction. Program execution continues at the memory address 0000 0000 00XX X000 where XXX is the suffix of the instruction, a number in the range 000 to 111 (in base 2). */
#define RST_111 0xFF  /* RST 111 (Restart). The contents of the program counter PC are pushed onto the stack, providing a return address for later use by a RETURN instruction. Program execution continues at the memory address 0000 0000 00XX X000 where XXX is the suffix of the instruction, a number in the range 000 to 111 (in base 2). */


/*** INTERRUPT FLIP-FLOP INSTRUCTIONS ***/

#define EI 0xFB  /* EI (Enable interrupts). This instruction sets the INTE flip-flop, enabling the CPU to recognize and respond to interrupts. */
#define DI 0xF3  /* DI (Disable interrupts). This instruction resets the INTE flip-flop, causing the CPU to ignore all interrupts. */


/*** INPUT/OUTPUT INSTRUCTIONS ***/

#define IN 0xDD  /* IN (Input). An eight-bit data byte is read from input device number exp and replaces the contents of the accumulator register A. */
#define OUT 0xD3  /* OUT (Output). The contents of the accumulator register A are arent to output device number exp. */


/*** HALT INSTRUCTIONS ***/

#define HLT 0x76  /* HLT (Halt). The program counter PC is incremented to the address of the next sequential instruction. The CPU then enters the STOPPED state and no further activity takes place until an interrupt occurs. */



/* -------------------------------------------------------------------------- |
 |                                                                            |
 |                      INSTRUCTION SUPPORTING FUNCTIONS                      |
 |                                                                            |
 | -------------------------------------------------------------------------- */

inline static uint16_t
pack_u16(uint8_t R_HI, uint8_t R_LO)
{
        return ((uint16_t)R_HI << 8) | (uint16_t) R_LO;
}

inline static void
update_sign_flag(i8080 *cpu, uint8_t x)
{
        flag_write(cpu, F_S, x & F_S);
}

inline static void
update_zero_flag(i8080 *cpu, uint8_t x)
{
        flag_write(cpu, F_Z, (x == 0) ? F_Z : 0);
}

inline static void
update_parity_flag(i8080 *cpu, uint16_t x)
{
        flag_write(cpu, F_P, parity(x) ? F_P : 0);
}

inline static void
update_carry_flag(i8080 *cpu, uint16_t res)
{
        flag_write(cpu, F_CY, (res > 0xFF) ? F_CY : 0);
}

inline static void
update_carry_flag_on_borrow(i8080 *cpu, uint16_t mend, uint16_t send)
{
        // Set the carry flag if the minuend is smaller than the subtrahend
        flag_write(cpu, F_CY, (mend < send) ? F_CY : 0);
}

inline static void
update_aux_carry_flag(i8080 *cpu, uint8_t x)
{
        flag_write(cpu, F_AC, (x & 0x10) ? F_AC : 0);
}

inline static void
update_aux_carry_flag_on_borrow(i8080 *cpu, uint16_t mend, uint16_t send)
{
        // Set the auxiliary carry flag if the lower nibble of the minuend
        // is smaller than the lower nibble of the subtrahend
        flag_write(cpu, F_AC, ((mend & 0x0F) < (send & 0x0F)) ? F_CY : 0);
}

inline static uint8_t
immediate_byte(i8080 *cpu)
{
        return cpu->mem[cpu->PC + 1];
}

inline static uint8_t *
immediate_pbyte(i8080 *cpu)
{
        return &cpu->mem[cpu->PC + 1];
}

inline static uint16_t
immediate_byte_pair_lo_first(i8080 *cpu)
{
        return pack_u16(cpu->mem[cpu->PC + 2], cpu->mem[cpu->PC + 1]);
}

inline static uint16_t
immediate_byte_pair_hi_first(i8080 *cpu)
{
        return pack_u16(cpu->mem[cpu->PC + 1], cpu->mem[cpu->PC + 2]);
}

inline static void
push_stack_PC(i8080 *cpu)
{
        cpu->mem[--cpu->SP] = (uint8_t)(cpu->PC >> 8);
        cpu->mem[--cpu->SP] = (uint8_t)(cpu->PC & 0x00FF);
}

inline static void
subroutine_return(i8080 *cpu)
{
        uint8_t ret_addr_lo = stack_pop(cpu);
        uint8_t ret_addr_hi = stack_pop(cpu);
        cpu->PC = pack_u16(ret_addr_hi, ret_addr_lo);
}

inline static void
call_from_immediate_data(i8080 *cpu)
{
        uint8_t addr_lo = cpu->mem[cpu->PC + 1];
        uint8_t addr_hi = cpu->mem[cpu->PC + 2];
        uint16_t addr_ret = cpu->PC + 3;
        uint8_t addr_ret_hi = (uint8_t)(addr_ret >> 8);
        uint8_t addr_ret_lo = (uint8_t)(addr_ret & 0x00FF);
        cpu->mem[--cpu->SP] = addr_ret_hi;
        cpu->mem[--cpu->SP] = addr_ret_lo;
        cpu->PC = pack_u16(addr_hi, addr_lo);
}

inline static void
write_mem(i8080 *cpu, uint8_t hi, uint8_t lo, uint8_t byte)
{
        cpu->mem[pack_u16(hi, lo)] = byte;
}

inline static uint8_t
read_mem(i8080 *cpu, uint8_t hi, uint8_t lo)
{
        return cpu->mem[pack_u16(hi, lo)];
}

inline static uint8_t
read_mem_HL(i8080 *cpu)
{
        return read_mem(cpu, cpu->H, cpu->L);
}

inline static uint8_t
read_mem_BC(i8080 *cpu)
{
        return read_mem(cpu, cpu->B, cpu->C);
}

inline static uint8_t
read_mem_DE(i8080 *cpu)
{
        return read_mem(cpu, cpu->D, cpu->E);
}

inline static uint8_t *
read_memp(i8080 *cpu, uint8_t hi, uint8_t lo)
{
        return &cpu->mem[pack_u16(hi, lo)];
}

inline static uint8_t *
read_memp_HL(i8080 *cpu)
{
        return read_memp(cpu, cpu->H, cpu->L);
}

inline static uint8_t *
read_memp_BC(i8080 *cpu)
{
        return read_memp(cpu, cpu->B, cpu->C);
}

inline static uint8_t *
read_memp_DE(i8080 *cpu)
{
        return read_memp(cpu, cpu->D, cpu->E);
}


/* -------------------------------------------------------------------------- |
 |                                                                            |
 |                         INSTRUCTION IMPLEMENTATIONS                        |
 |                                                                            |
 | -------------------------------------------------------------------------- */

inline static void
OP_ADC(i8080 *cpu, uint8_t *reg)
{
        uint8_t carry = flag_get(cpu, F_CY);
        uint16_t sum = cpu->A + *reg + carry;
        cpu->A = sum & 0xFF;
        uint8_t half = (cpu->A & 0xF) + (*reg & 0xF) + carry;
        update_aux_carry_flag(cpu, half);
        update_carry_flag(cpu, sum);
        update_sign_flag(cpu, cpu->A);
        update_zero_flag(cpu, cpu->A);
        update_parity_flag(cpu, sum);
}

inline static void
OP_ADD(i8080 *cpu, uint8_t *reg)
{
        uint16_t sum = cpu->A + *reg;
        cpu->A = sum & 0xFF;
        uint8_t half = (cpu->A & 0xF) + (*reg & 0xF);
        update_aux_carry_flag(cpu, half);
        update_carry_flag(cpu, sum);
        update_sign_flag(cpu, cpu->A);
        update_zero_flag(cpu, cpu->A);
        update_parity_flag(cpu, sum);
}

inline static void
OP_ACI(i8080 *cpu)
{
        uint8_t carry = flag_get(cpu, F_CY);
        uint8_t byte = cpu->mem[cpu->PC + 1];
        uint16_t sum = cpu->A + byte + carry;
        cpu->A = sum & 0xFF;
        uint8_t half = (cpu->A & 0xF) + byte + carry;
        update_carry_flag(cpu, sum);
        update_sign_flag(cpu, sum);
        update_zero_flag(cpu, sum);
        update_parity_flag(cpu, sum);
        update_aux_carry_flag(cpu, half);
}

inline static void
OP_ADI(i8080 *cpu)
{
        uint8_t byte = cpu->mem[cpu->PC + 1];
        uint16_t sum = cpu->A + byte;
        cpu->A = sum & 0xFF;
        uint8_t half = (cpu->A & 0xF) + byte;
        update_carry_flag(cpu, sum);
        update_sign_flag(cpu, sum);
        update_zero_flag(cpu, sum);
        update_parity_flag(cpu, sum);
        update_aux_carry_flag(cpu, half);
}

inline static void
OP_ANA(i8080 *cpu, uint8_t reg)
{
        cpu->A &= reg;
        flag_clear(cpu, F_CY);
        update_sign_flag(cpu, cpu->A);
        update_zero_flag(cpu, cpu->A);
        update_parity_flag(cpu, cpu->A);
}

inline static void
OP_ANI(i8080 *cpu)
{
        cpu->A &= cpu->mem[cpu->PC + 1];
        flag_clear(cpu, F_CY);
        update_sign_flag(cpu, cpu->A);
        update_zero_flag(cpu, cpu->A);
        update_parity_flag(cpu, cpu->A);
}

inline static void
OP_CALL(i8080 *cpu)
{
        call_from_immediate_data(cpu);
}

inline static void
OP_CC(i8080 *cpu)
{
        if (flag_get(cpu, F_CY))
                call_from_immediate_data(cpu);
}

inline static void
OP_CM(i8080 *cpu)
{
        if (flag_get(cpu, F_S))
                call_from_immediate_data(cpu);
}

inline static void
OP_CMP(i8080 *cpu, uint8_t *reg)
{
        uint16_t sum = cpu->A - *reg;
        update_carry_flag_on_borrow(cpu, cpu->A, *reg);
        update_aux_carry_flag_on_borrow(cpu, cpu->A, *reg);
        update_sign_flag(cpu, sum);
        update_zero_flag(cpu, sum);
        update_parity_flag(cpu, sum);
}

inline static void
OP_CNC(i8080 *cpu)
{
        if (!flag_get(cpu, F_CY))
                call_from_immediate_data(cpu);
}

inline static void
OP_CNZ(i8080 *cpu)
{
        if (!flag_get(cpu, F_Z))
                call_from_immediate_data(cpu);
}

inline static void
OP_CP(i8080 *cpu)
{
        if (!flag_get(cpu, F_S))
                call_from_immediate_data(cpu);
}

inline static void
OP_CPE(i8080 *cpu)
{
        if (flag_get(cpu, F_P))
                call_from_immediate_data(cpu);
}

inline static void
OP_CPI(i8080 *cpu)
{
        uint8_t byte = cpu->mem[cpu->PC + 1];
        uint16_t sum = cpu->A - byte;
        update_carry_flag_on_borrow(cpu, cpu->A, byte);
        update_aux_carry_flag_on_borrow(cpu, cpu->A, byte);
        update_sign_flag(cpu, sum);
        update_zero_flag(cpu, sum);
        update_parity_flag(cpu, sum);
}

inline static void
OP_CPO(i8080 *cpu)
{
        if (!flag_get(cpu, F_P))
                call_from_immediate_data(cpu);
}

inline static void
OP_CZ(i8080 *cpu)
{
        if (flag_get(cpu, F_Z))
                call_from_immediate_data(cpu);
}

inline static void
OP_DAA(i8080 *cpu)
{
        uint8_t corr = 0;

        if (((cpu->A & 0x0F) > 9) || flag_get(cpu, F_AC)) {
                uint8_t lo = cpu->A & 0x0F;
                uint16_t inc = cpu->A + 6;
                cpu->A = inc & 0xFF;
                flag_write(cpu, F_AC, (lo + 6 > 0x0F) ? F_AC : 0);
        }

        if (((cpu->A >> 4) > 9) || flag_get(cpu, F_CY)) {
                uint16_t hi = cpu->A & 0xF0;
                uint16_t tmp = ((cpu->A >> 4) + 6) & 0x0F;
                cpu->A = (cpu->A & ~0xF0) | tmp;
                flag_write(cpu, F_CY, (hi + 6 > 0xFF) ? F_CY : 0);

        }
        update_sign_flag(cpu, cpu->A);
        update_zero_flag(cpu, cpu->A);
        update_parity_flag(cpu, cpu->A);
}

inline static void
OP_DAD_BC(i8080 *cpu)
{
        uint16_t HL = pack_u16(cpu->H, cpu->L);
        uint16_t BC = pack_u16(cpu->B, cpu->C);
        uint32_t sum = HL + BC;
        flag_write(cpu, F_CY, (sum > 0xFFFF) ? F_CY : 0);
        cpu->H = sum & 0x00F0;
        cpu->L = sum & 0x000F;
}

inline static void
OP_DAD_DE(i8080 *cpu)
{
        uint16_t HL = pack_u16(cpu->H, cpu->L);
        uint16_t DE = pack_u16(cpu->D, cpu->E);
        uint32_t sum = HL + DE;
        flag_write(cpu, F_CY, (sum > 0xFFFF) ? F_CY : 0);
        cpu->H = sum & 0x00F0;
        cpu->L = sum & 0x000F;
}

inline static void
OP_DAD_HL(i8080 *cpu)
{
        uint16_t HL = pack_u16(cpu->H, cpu->L);
        uint32_t sum = HL + HL;
        flag_write(cpu, F_CY, (sum > 0xFFFF) ? F_CY : 0);
        cpu->H = sum & 0x00F0;
        cpu->L = sum & 0x000F;
}

inline static void
OP_DAD_SP(i8080 *cpu)
{
        uint16_t HL = pack_u16(cpu->H, cpu->L);
        uint32_t sum = HL + cpu->SP;
        flag_write(cpu, F_CY, (sum > 0xFFFF) ? F_CY : 0);
        cpu->H = sum & 0x00F0;
        cpu->L = sum & 0x000F;
}

inline static void
OP_DCR(i8080 *cpu, uint8_t *reg)
{
        --(*reg);
        update_sign_flag(cpu, *reg);
        update_zero_flag(cpu, *reg);
        update_parity_flag(cpu, *reg);
        update_aux_carry_flag(cpu, *reg);
}

inline static void
OP_DCX(i8080 *cpu, uint16_t *reg)
{
        --(*reg);
}

inline static void
OP_DCX_PAIR(i8080 *cpu, uint8_t *rega, uint8_t *regb)
{
        uint16_t tmp = pack_u16(*rega, *regb) - 1;
        *rega = (uint8_t)(tmp >> 8);
        *regb = (uint8_t)(tmp & 0xFF);
}

inline static void
OP_DI(i8080 *cpu)
{
        cpu->INTE = false;
}

inline static void
OP_EI(i8080 *cpu)
{
        cpu->INTE = true;
}

inline static void
OP_HLT(i8080 *cpu)
{
        cpu->halted = true;
}

inline static void
OP_IN(i8080 *cpu, uint8_t *reg)
{
}

inline static void
OP_INR(i8080 *cpu, uint8_t *reg)
{
        uint8_t carry = flag_get(cpu, F_CY);
        uint16_t ans = *reg + carry + 1;
        *reg = ans & 0xFF;
        uint8_t half = (*reg & 0xF) + carry + 1;
        update_carry_flag(cpu, ans);
        update_sign_flag(cpu, ans);
        update_zero_flag(cpu, ans);
        update_parity_flag(cpu, ans);
        update_aux_carry_flag(cpu, half);
}

inline static void
OP_INX(i8080 *cpu, uint16_t *reg)
{
        ++(*reg);
}

inline static void
OP_INX_PAIR(i8080 *cpu, uint8_t *rega, uint8_t *regb)
{
        uint16_t tmp = pack_u16(*rega, *regb) + 1;
        *rega = (uint8_t)(tmp >> 8);
        *regb = (uint8_t)(tmp & 0xFF);
}

inline static void
OP_JC(i8080 *cpu)
{
        if (flag_get(cpu, F_CY))
                cpu->PC = immediate_byte_pair_lo_first(cpu);
}

inline static void
OP_JM(i8080 *cpu)
{
        if (flag_get(cpu, F_S))
                cpu->PC = immediate_byte_pair_lo_first(cpu);
}

inline static void
OP_JMP(i8080 *cpu)
{
        cpu->PC = immediate_byte_pair_lo_first(cpu);
}

inline static void
OP_JNC(i8080 *cpu)
{
        if (!flag_get(cpu, F_CY))
                cpu->PC = immediate_byte_pair_lo_first(cpu);
}

inline static void
OP_JNZ(i8080 *cpu)
{
        if (!flag_get(cpu, F_Z))
                cpu->PC = immediate_byte_pair_lo_first(cpu);
}

inline static void
OP_JP(i8080 *cpu)
{
        if (!flag_get(cpu, F_S))
                cpu->PC = immediate_byte_pair_lo_first(cpu);
}

inline static void
OP_JPE(i8080 *cpu)
{
        if (flag_get(cpu, F_P))
                cpu->PC = immediate_byte_pair_lo_first(cpu);
}

inline static void
OP_JPO(i8080 *cpu)
{
        if (!flag_get(cpu, F_P))
                cpu->PC = immediate_byte_pair_lo_first(cpu);
}

inline static void
OP_JZ(i8080 *cpu)
{
        if (flag_get(cpu, F_Z))
                cpu->PC = immediate_byte_pair_lo_first(cpu);
}

inline static void
OP_LDA(i8080 *cpu)
{
        cpu->A = cpu->mem[immediate_byte_pair_lo_first(cpu)];
}

inline static void
OP_LDAX(i8080 *cpu, uint8_t *mem)
{
        cpu->A = *mem;
}

inline static void
OP_LHLD(i8080 *cpu)
{
        uint16_t addr = immediate_byte_pair_lo_first(cpu);
        cpu->L = cpu->mem[addr];
        cpu->H = cpu->mem[addr + 1];
}

inline static void
OP_ORA(i8080 *cpu, uint8_t *req)
{
        cpu->A |= *req;
        flag_clear(cpu, F_CY);
        update_sign_flag(cpu, cpu->A);
        update_zero_flag(cpu, cpu->A);
        update_parity_flag(cpu, cpu->A);
}

inline static void
OP_ORI(i8080 *cpu)
{
        cpu->A |= cpu->mem[cpu->PC + 1];
        flag_clear(cpu, F_CY);
        update_sign_flag(cpu, cpu->A);
        update_zero_flag(cpu, cpu->A);
        update_parity_flag(cpu, cpu->A);
}

inline static void
OP_MOV(uint8_t *dst, uint8_t *src)
{
        *dst = *src;
}

inline static void
OP_MVI(i8080 *cpu, uint8_t *req)
{
        *req = cpu->mem[cpu->PC++];
}

inline static void
OP_OUT(i8080 *cpu, uint8_t *reg)
{
}

inline static void
OP_PCHL(i8080 *cpu)
{
        cpu->PC = pack_u16(cpu->H, cpu->L);
}

inline static void
OP_POP_BC(i8080 *cpu)
{
        cpu->C = cpu->mem[cpu->SP++];
        cpu->B = cpu->mem[cpu->SP++];
}

inline static void
OP_POP_DE(i8080 *cpu)
{
        cpu->E = cpu->mem[cpu->SP++];
        cpu->D = cpu->mem[cpu->SP++];
}

inline static void
OP_POP_HL(i8080 *cpu)
{
        cpu->L = cpu->mem[cpu->SP++];
        cpu->H = cpu->mem[cpu->SP++];
}

inline static void
OP_POP_PSW(i8080 *cpu)
{
        cpu->F = cpu->mem[cpu->SP++];
        cpu->A = cpu->mem[cpu->SP++];
}

inline static void
OP_PUSH_BC(i8080 *cpu)
{
        cpu->mem[--cpu->SP] = cpu->B;
        cpu->mem[--cpu->SP] = cpu->C;
}

inline static void
OP_PUSH_DE(i8080 *cpu)
{
        cpu->mem[--cpu->SP] = cpu->D;
        cpu->mem[--cpu->SP] = cpu->E;
}

inline static void
OP_PUSH_HL(i8080 *cpu)
{
        cpu->mem[--cpu->SP] = cpu->H;
        cpu->mem[--cpu->SP] = cpu->L;
}

inline static void
OP_PUSH_PSW(i8080 *cpu)
{
        cpu->mem[--cpu->SP] = cpu->A;
        cpu->mem[--cpu->SP] = cpu->F;
}

inline static void
OP_RAL(i8080 *cpu)
{
        uint8_t hi = cpu->A >> 7;
        flag_write(cpu, F_CY, hi ? F_CY : 0);
        cpu->A = ((cpu->A >> 1) & ~(1 << 7)) | (hi << 7);

}

inline static void
OP_RAR(i8080 *cpu)
{
        uint8_t lo = cpu->A & 0x01;
        flag_write(cpu, F_CY, lo ? F_CY : 0);
        cpu->A = ((cpu->A >> 1) & ~(1 << 7)) | (lo << 7);

}

inline static void
OP_RC(i8080 *cpu)
{
        if (flag_get(cpu, F_CY))
                subroutine_return(cpu);
}

inline static void
OP_RET(i8080 *cpu)
{
        subroutine_return(cpu);
}

inline static void
OP_RLC(i8080 *cpu)
{
        uint8_t hi = cpu->A >> 7;
        flag_write(cpu, F_CY, hi ? F_CY : 0);
        cpu->A = ((cpu->A << 1) & ~0x01) | hi;

}

inline static void
OP_RM(i8080 *cpu)
{
        if (flag_get(cpu, F_S))
                subroutine_return(cpu);
}

inline static void
OP_RNC(i8080 *cpu)
{
        if (!flag_get(cpu, F_CY))
                subroutine_return(cpu);
}

inline static void
OP_RP(i8080 *cpu)
{
        if (!flag_get(cpu, F_S))
                subroutine_return(cpu);
}

inline static void
OP_RPE(i8080 *cpu)
{
        if (flag_get(cpu, F_P))
                subroutine_return(cpu);
}

inline static void
OP_RPO(i8080 *cpu)
{
        if (!flag_get(cpu, F_P))
                subroutine_return(cpu);
}

inline static void
OP_RRC(i8080 *cpu)
{
        uint8_t lo = cpu->A & 0x01;
        flag_write(cpu, F_CY, lo ? F_CY : 0);
        cpu->A = ((cpu->A >> 1) & ~(1 << 7)) | (lo << 7);

}

inline static void
OP_RST_000(i8080 *cpu)
{
        push_stack_PC(cpu);
        cpu->PC = 0x00C7;
}

inline static void
OP_RST_001(i8080 *cpu)
{
        push_stack_PC(cpu);
        cpu->PC = 0x00CF;
}

inline static void
OP_RST_010(i8080 *cpu)
{
        push_stack_PC(cpu);
        cpu->PC = 0x00D7;
}

inline static void
OP_RST_100(i8080 *cpu)
{
        push_stack_PC(cpu);
        cpu->PC = 0x00E7;
}

inline static void
OP_RST_101(i8080 *cpu)
{
        push_stack_PC(cpu);
        cpu->PC = 0x00EF;
}

inline static void
OP_RST_110(i8080 *cpu)
{
        push_stack_PC(cpu);
        cpu->PC = 0x00F7;
}

inline static void
OP_RST_111(i8080 *cpu)
{
        push_stack_PC(cpu);
        cpu->PC = 0x00FF;
}

inline static void
OP_RZ(i8080 *cpu)
{
        if (flag_get(cpu, F_Z))
                subroutine_return(cpu);
}

inline static void
OP_RNZ(i8080 *cpu)
{
        if (!flag_get(cpu, F_Z))
                subroutine_return(cpu);
}

inline static void
OP_SBB(i8080 *cpu, uint8_t *reg)
{
        uint16_t send = *reg + flag_get(cpu, F_CY);
        uint16_t sum = cpu->A - send;
        update_carry_flag_on_borrow(cpu, cpu->A, send);
        update_aux_carry_flag_on_borrow(cpu, cpu->A, send);
        cpu->A = sum & 0xFF;
        update_sign_flag(cpu, sum);
        update_zero_flag(cpu, sum);
        update_parity_flag(cpu, sum);
}

inline static void
OP_SBI(i8080 *cpu)
{
        uint8_t carry = flag_get(cpu, F_CY);
        uint16_t byte = cpu->mem[cpu->PC + 1] + carry;
        uint16_t sum = cpu->A - byte;
        update_carry_flag_on_borrow(cpu, cpu->A, byte);
        update_aux_carry_flag_on_borrow(cpu, cpu->A, byte);
        cpu->A = sum & 0xFF;
        update_sign_flag(cpu, sum);
        update_zero_flag(cpu, sum);
        update_parity_flag(cpu, sum);
}

inline static void
OP_SHLD(i8080 *cpu)
{
        uint16_t addr = immediate_byte_pair_lo_first(cpu);
        cpu->mem[addr] = cpu->L;
        cpu->mem[addr + 1] = cpu->H;
}

inline static void
OP_SPHL(i8080 *cpu)
{
        cpu->SP = pack_u16(cpu->H, cpu->L);
}

inline static void
OP_STA(i8080 *cpu)
{
        cpu->mem[immediate_byte_pair_lo_first(cpu)] = cpu->A;
}

inline static void
OP_STAX(i8080 *cpu, uint8_t *mem)
{
        *mem = cpu->A;
}

inline static void
OP_SUB(i8080 *cpu, uint8_t *reg)
{
        uint16_t sum = cpu->A - *reg;
        update_carry_flag_on_borrow(cpu, cpu->A, *reg);
        update_aux_carry_flag_on_borrow(cpu, cpu->A, *reg);
        cpu->A = sum & 0xFF;
        update_sign_flag(cpu, sum);
        update_zero_flag(cpu, sum);
        update_parity_flag(cpu, sum);
}

inline static void
OP_SUI(i8080 *cpu)
{
        uint8_t byte = cpu->mem[cpu->PC + 1];
        uint16_t sum = cpu->A - byte;
        update_carry_flag_on_borrow(cpu, cpu->A, byte);
        update_aux_carry_flag_on_borrow(cpu, cpu->A, byte);
        cpu->A = sum & 0xFF;
        update_sign_flag(cpu, sum);
        update_zero_flag(cpu, sum);
        update_parity_flag(cpu, sum);
}

inline static void
OP_XCHG(i8080 *cpu)
{
        uint8_t tmp_hi = cpu->H;
        uint8_t tmp_lo = cpu->L;
        cpu->H = cpu->D;
        cpu->L = cpu->E;
        cpu->D = tmp_hi;
        cpu->E = tmp_lo;
}

inline static void
OP_XTHL(i8080 *cpu)
{
        cpu->L = cpu->mem[cpu->SP];
        cpu->H = cpu->mem[cpu->SP + 1];
}

inline static void
OP_XRA(i8080 *cpu, uint8_t *req)
{
        cpu->A ^= *req;
        flag_clear(cpu, F_CY);
        update_sign_flag(cpu, cpu->A);
        update_zero_flag(cpu, cpu->A);
        update_parity_flag(cpu, cpu->A);
        update_aux_carry_flag(cpu, cpu->A);
}

inline static void
OP_XRI(i8080 *cpu)
{
        cpu->A ^= cpu->mem[cpu->PC + 1];
        flag_clear(cpu, F_CY);
        update_sign_flag(cpu, cpu->A);
        update_zero_flag(cpu, cpu->A);
        update_parity_flag(cpu, cpu->A);
}


/* -------------------------------------------------------------------------- |
 |                                                                            |
 |                                EMULATOR CORE                               |
 |                                                                            |
 | -------------------------------------------------------------------------- */

void
request_interrupt(i8080 *cpu, int int_num)
{
        cpu->int_pending = int_num;
}

void
handle_interrupt(i8080 *cpu)
{
        cpu->INTE = false;

        cpu->mem[--cpu->SP] = (cpu->PC >> 8) & 0xFF;
        cpu->mem[--cpu->SP] = cpu->PC & 0xFF;

        cpu->PC = 8 * cpu->int_pending;

        cpu->int_pending = 0;
}

void
emulate(i8080 *cpu)
{
        opcode op;
        for (;;) {
                if (cpu->halted)
                        continue;

                if (cpu->INTE && cpu->int_pending != 0)
                        handle_interrupt(cpu);

                op = cpu->mem[cpu->PC++];
                dispatch(cpu, op);
        }
}

void
init(i8080 *cpu, const char *path)
{
        memset(cpu, 0, sizeof *cpu);
        load(cpu, path);
        cpu->PC = BEGIN_ADDR;
}

static void
load(i8080 *cpu, const char *path)
{
        FILE *file = fopen(path, "rb");
        if (!file) {
                perror(path);
                exit(1);
        }

        size_t n = fread(cpu->mem + BEGIN_ADDR, 1, ADDR_SPACE_SZ - BEGIN_ADDR, file);
        fclose(file);
}

static inline void
dispatch(i8080 *cpu, opcode op)
{
        switch (op) {
                case ADC_B: { OP_ADC(cpu, &cpu->B); break; }
                case ADC_C: { OP_ADC(cpu, &cpu->C); break; }
                case ADC_D: { OP_ADC(cpu, &cpu->D); break; }
                case ADC_E: { OP_ADC(cpu, &cpu->E); break; }
                case ADC_H: { OP_ADC(cpu, &cpu->H); break; }
                case ADC_L: { OP_ADC(cpu, &cpu->L); break; }
                case ADC_M: { OP_ADC(cpu, read_memp_HL(cpu)); break; }
                case ADC_A: { OP_ADC(cpu, &cpu->A); break; }

                case ADD_B: { OP_ADD(cpu, &cpu->B); break; }
                case ADD_C: { OP_ADD(cpu, &cpu->C); break; }
                case ADD_D: { OP_ADD(cpu, &cpu->D); break; }
                case ADD_E: { OP_ADD(cpu, &cpu->E); break; }
                case ADD_H: { OP_ADD(cpu, &cpu->H); break; }
                case ADD_L: { OP_ADD(cpu, &cpu->L); break; }
                case ADD_M: { OP_ADD(cpu, read_memp_HL(cpu)); break; }
                case ADD_A: { OP_ADD(cpu, &cpu->A); break; }

                case ADI: { OP_ADI(cpu); break; }
                case ACI: { OP_ACI(cpu); break; }

                case ANA_B: { OP_ANA(cpu, cpu->B); break; }
                case ANA_C: { OP_ANA(cpu, cpu->C); break; }
                case ANA_D: { OP_ANA(cpu, cpu->D); break; }
                case ANA_E: { OP_ANA(cpu, cpu->E); break; }
                case ANA_H: { OP_ANA(cpu, cpu->H); break; }
                case ANA_L: { OP_ANA(cpu, cpu->L); break; }
                case ANA_M: { OP_ANA(cpu, read_mem_HL(cpu)); break; }
                case ANA_A: { OP_ANA(cpu, cpu->A); break; }

                case ANI: { OP_ANI(cpu); break; }

                case CMP_B: { OP_CMP(cpu, &cpu->B); break; }
                case CMP_C: { OP_CMP(cpu, &cpu->C); break; }
                case CMP_D: { OP_CMP(cpu, &cpu->D); break; }
                case CMP_E: { OP_CMP(cpu, &cpu->E); break; }
                case CMP_H: { OP_CMP(cpu, &cpu->H); break; }
                case CMP_L: { OP_CMP(cpu, &cpu->L); break; }
                case CMP_M: { OP_CMP(cpu, read_memp_HL(cpu)); break; }
                case CMP_A: { OP_CMP(cpu, &cpu->A); break; }

                case CMA: { cpu->A ^= 0xFF; break; }
                case CMC: { flag_toggle(cpu, F_CY); break; }
                case CPI: { OP_CPI(cpu); break; }

                case DAA: { OP_DAA(cpu); break; }

                case DAD_BC: { OP_DAD_BC(cpu); break; }
                case DAD_DE: { OP_DAD_DE(cpu); break; }
                case DAD_HL: { OP_DAD_HL(cpu); break; }
                case DAD_SP: { OP_DAD_SP(cpu); break; }

                case DCX_BC: { OP_DCX_PAIR(cpu, &cpu->B, &cpu->C); break; }
                case DCX_DE: { OP_DCX_PAIR(cpu, &cpu->D, &cpu->E); break; }
                case DCX_HL: { OP_DCX_PAIR(cpu, &cpu->H, &cpu->L); break; }
                case DCX_SP: { OP_DCX(cpu, &cpu->SP); break; }

                case DCR_B: { OP_DCR(cpu, &cpu->B); break; }
                case DCR_C: { OP_DCR(cpu, &cpu->C); break; }
                case DCR_D: { OP_DCR(cpu, &cpu->D); break; }
                case DCR_E: { OP_DCR(cpu, &cpu->E); break; }
                case DCR_H: { OP_DCR(cpu, &cpu->H); break; }
                case DCR_L: { OP_DCR(cpu, &cpu->L); break; }
                case DCR_M: { OP_DCR(cpu, read_memp_HL(cpu)); break; }
                case DCR_A: { OP_DCR(cpu, &cpu->A); break; }

                case DI: { OP_DI(cpu); break; }

                case CALL: { OP_CALL(cpu); break; }
                case CC: { OP_CC(cpu); break; }
                case CNC: { OP_CNC(cpu); break; }
                case CZ: { OP_CZ(cpu); break; }
                case CNZ: { OP_CNZ(cpu); break; }
                case CM: { OP_CM(cpu); break; }
                case CP: { OP_CP(cpu); break; }
                case CPE: { OP_CPE(cpu); break; }
                case CPO: { OP_CPO(cpu); break; }

                case EI: { OP_EI(cpu); break; }
                case HLT: { OP_HLT(cpu); break; }

                case INR_B: { OP_INR(cpu, &cpu->B); break; }
                case INR_C: { OP_INR(cpu, &cpu->C); break; }
                case INR_D: { OP_INR(cpu, &cpu->D); break; }
                case INR_E: { OP_INR(cpu, &cpu->E); break; }
                case INR_H: { OP_INR(cpu, &cpu->H); break; }
                case INR_L: { OP_INR(cpu, &cpu->L); break; }
                case INR_M: { OP_INR(cpu, read_memp_HL(cpu)); break; }
                case INR_A: { OP_INR(cpu, &cpu->A); break; }

                case INX_BC: { OP_INX_PAIR(cpu, &cpu->B, &cpu->C); break; }
                case INX_DE: { OP_INX_PAIR(cpu, &cpu->D, &cpu->E); break; }
                case INX_HL: { OP_INX_PAIR(cpu, &cpu->H, &cpu->L); break; }
                case INX_SP: { OP_INX(cpu, &cpu->SP); break; }

                case JC: { OP_JC(cpu); break; }
                case JMP: { OP_JMP(cpu); break; }
                case JM: { OP_JM(cpu); break; }
                case JNC: { OP_JNC(cpu); break; }
                case JNZ: { OP_JNZ(cpu); break; }
                case JP: { OP_JP(cpu); break; }
                case JPE: { OP_JPE(cpu); break; }
                case JPO: { OP_JPO(cpu); break; }
                case JZ: { OP_JZ(cpu); break; }

                case LDA: { OP_LDA(cpu); break; }
                case LDAX_BC: { OP_LDAX(cpu, read_memp_BC(cpu)); break; }
                case LDAX_DE: { OP_LDAX(cpu, read_memp_DE(cpu)); break; }
                case LHLD: { OP_LHLD(cpu); break; }
                case NOP: { break; }

                case MOV_A_B: { OP_MOV(&cpu->A, &cpu->B); break; }
                case MOV_A_C: { OP_MOV(&cpu->A, &cpu->C); break; }
                case MOV_A_D: { OP_MOV(&cpu->A, &cpu->D); break; }
                case MOV_A_E: { OP_MOV(&cpu->A, &cpu->E); break; }
                case MOV_A_H: { OP_MOV(&cpu->A, &cpu->H); break; }
                case MOV_A_L: { OP_MOV(&cpu->A, &cpu->L); break; }
                case MOV_A_M: { OP_MOV(&cpu->A, read_memp_HL(cpu)); break; }
                case MOV_A_A: { break; }

                case MOV_B_B: { break; }
                case MOV_B_C: { OP_MOV(&cpu->B, &cpu->C); break; }
                case MOV_B_D: { OP_MOV(&cpu->B, &cpu->D); break; }
                case MOV_B_E: { OP_MOV(&cpu->B, &cpu->E); break; }
                case MOV_B_H: { OP_MOV(&cpu->B, &cpu->H); break; }
                case MOV_B_L: { OP_MOV(&cpu->B, &cpu->L); break; }
                case MOV_B_M: { OP_MOV(&cpu->B, read_memp_HL(cpu)); break; }
                case MOV_B_A: { OP_MOV(&cpu->B, &cpu->A); break; }

                case MOV_C_B: { OP_MOV(&cpu->C, &cpu->B); break; }
                case MOV_C_C: { break; }
                case MOV_C_D: { OP_MOV(&cpu->C, &cpu->D); break; }
                case MOV_C_E: { OP_MOV(&cpu->C, &cpu->E); break; }
                case MOV_C_H: { OP_MOV(&cpu->C, &cpu->H); break; }
                case MOV_C_L: { OP_MOV(&cpu->C, &cpu->L); break; }
                case MOV_C_M: { OP_MOV(&cpu->C, read_memp_HL(cpu)); break; }
                case MOV_C_A: { OP_MOV(&cpu->C, &cpu->A); break; }

                case MOV_D_B: { OP_MOV(&cpu->D, &cpu->B); break; }
                case MOV_D_C: { OP_MOV(&cpu->D, &cpu->C); break; }
                case MOV_D_D: { break; }
                case MOV_D_E: { OP_MOV(&cpu->D, &cpu->E); break; }
                case MOV_D_H: { OP_MOV(&cpu->D, &cpu->H); break; }
                case MOV_D_L: { OP_MOV(&cpu->D, &cpu->L); break; }
                case MOV_D_M: { OP_MOV(&cpu->D, read_memp_HL(cpu)); break; }
                case MOV_D_A: { OP_MOV(&cpu->D, &cpu->A); break; }

                case MOV_E_B: { OP_MOV(&cpu->E, &cpu->B); break; }
                case MOV_E_C: { OP_MOV(&cpu->E, &cpu->C); break; }
                case MOV_E_D: { OP_MOV(&cpu->E, &cpu->D); break; }
                case MOV_E_E: { break; }
                case MOV_E_H: { OP_MOV(&cpu->E, &cpu->H); break; }
                case MOV_E_L: { OP_MOV(&cpu->E, &cpu->L); break; }
                case MOV_E_M: { OP_MOV(&cpu->E, read_memp_HL(cpu)); break; }
                case MOV_E_A: { OP_MOV(&cpu->E, &cpu->A); break; }

                case MOV_H_B: { OP_MOV(&cpu->H, &cpu->B); break; }
                case MOV_H_C: { OP_MOV(&cpu->H, &cpu->C); break; }
                case MOV_H_D: { OP_MOV(&cpu->H, &cpu->D); break; }
                case MOV_H_E: { OP_MOV(&cpu->H, &cpu->E); break; }
                case MOV_H_H: { break; }
                case MOV_H_L: { OP_MOV(&cpu->H, &cpu->L); break; }
                case MOV_H_M: { OP_MOV(&cpu->H, read_memp_HL(cpu)); break; }
                case MOV_H_A: { OP_MOV(&cpu->H, &cpu->A); break; }

                case MOV_L_B: { OP_MOV(&cpu->L, &cpu->B); break; }
                case MOV_L_C: { OP_MOV(&cpu->L, &cpu->C); break; }
                case MOV_L_D: { OP_MOV(&cpu->L, &cpu->D); break; }
                case MOV_L_E: { OP_MOV(&cpu->L, &cpu->E); break; }
                case MOV_L_H: { OP_MOV(&cpu->L, &cpu->L); break; }
                case MOV_L_L: { break; }
                case MOV_L_M: { OP_MOV(&cpu->L, read_memp_HL(cpu)); break; }
                case MOV_L_A: { OP_MOV(&cpu->L, &cpu->A); break; }

                case MOV_M_B: { OP_MOV(read_memp_HL(cpu), &cpu->B); break; }
                case MOV_M_C: { OP_MOV(read_memp_HL(cpu), &cpu->C); break; }
                case MOV_M_D: { OP_MOV(read_memp_HL(cpu), &cpu->D); break; }
                case MOV_M_E: { OP_MOV(read_memp_HL(cpu), &cpu->E); break; }
                case MOV_M_H: { OP_MOV(read_memp_HL(cpu), &cpu->H); break; }
                case MOV_M_L: { OP_MOV(read_memp_HL(cpu), &cpu->L); break; }
                case MOV_M_A: { OP_MOV(read_memp_HL(cpu), &cpu->A); break; }

                case MVI_B: { OP_MVI(cpu, &cpu->B); break; }
                case MVI_C: { OP_MVI(cpu, &cpu->C); break; }
                case MVI_D: { OP_MVI(cpu, &cpu->D); break; }
                case MVI_E: { OP_MVI(cpu, &cpu->E); break; }
                case MVI_H: { OP_MVI(cpu, &cpu->H); break; }
                case MVI_L: { OP_MVI(cpu, &cpu->L); break; }
                case MVI_M: { OP_MVI(cpu, read_memp_HL(cpu)); break; }
                case MVI_A: { OP_MVI(cpu, &cpu->A); break; }

                case RST_000: { OP_RST_000(cpu); break; }
                case RST_001: { OP_RST_001(cpu); break; }
                case RST_010: { OP_RST_010(cpu); break; }
                case RST_100: { OP_RST_100(cpu); break; }
                case RST_101: { OP_RST_101(cpu); break; }
                case RST_110: { OP_RST_110(cpu); break; }
                case RST_111: { OP_RST_111(cpu); break; }

                case ORA_B: { OP_ORA(cpu, &cpu->B); break; }
                case ORA_C: { OP_ORA(cpu, &cpu->C); break; }
                case ORA_D: { OP_ORA(cpu, &cpu->D); break; }
                case ORA_E: { OP_ORA(cpu, &cpu->E); break; }
                case ORA_H: { OP_ORA(cpu, &cpu->H); break; }
                case ORA_L: { OP_ORA(cpu, &cpu->L); break; }
                case ORA_M: { OP_ORA(cpu, read_memp_HL(cpu)); break; }
                case ORA_A: { OP_ORA(cpu, &cpu->A); break; }

                case ORI: { OP_ORI(cpu); break; }
                case PCHL: { OP_PCHL(cpu); break; }

                case POP_BC: { OP_POP_BC(cpu); break; }
                case POP_DE: { OP_POP_DE(cpu); break; }
                case POP_HL: { OP_POP_HL(cpu); break; }
                case POP_PSW: { OP_POP_PSW(cpu); break; }

                case PUSH_BC: { OP_PUSH_BC(cpu); break; }
                case PUSH_DE: { OP_PUSH_DE(cpu); break; }
                case PUSH_HL: { OP_PUSH_HL(cpu); break; }
                case PUSH_PSW: { OP_PUSH_PSW(cpu); break; }

                case RAL: { OP_RAL(cpu); break; }
                case RAR: { OP_RAR(cpu); break; }
                case RC: { OP_RC(cpu); break; }
                case RET: { OP_RET(cpu); break; }
                case RLC: { OP_RLC(cpu); break; }
                case RM: { OP_RM(cpu); break; }
                case RNC: { OP_RNC(cpu); break; }
                case RNZ: { OP_RNZ(cpu); break; }
                case RP: { OP_RP(cpu); break; }
                case RPE: { OP_RPE(cpu); break; }
                case RPO: { OP_RPO(cpu); break; }
                case RRC: { OP_RRC(cpu); break; }
                case RZ: { OP_RZ(cpu); break; }

                case SBB_B: { OP_SBB(cpu, &cpu->B); break; }
                case SBB_C: { OP_SBB(cpu, &cpu->C); break; }
                case SBB_D: { OP_SBB(cpu, &cpu->D); break; }
                case SBB_E: { OP_SBB(cpu, &cpu->E); break; }
                case SBB_H: { OP_SBB(cpu, &cpu->H); break; }
                case SBB_L: { OP_SBB(cpu, &cpu->L); break; }
                case SBB_M: { OP_SBB(cpu, read_memp_HL(cpu)); break; }
                case SBB_A: { OP_SBB(cpu, &cpu->A); break; }

                case SBI: { OP_SBI(cpu); break; }
                case SHLD: { OP_SHLD(cpu); break; }
                case SPHL: { OP_SPHL(cpu); break; }
                case STC: { flag_set(cpu, F_CY); break; }

                case SUB_B: { OP_SUB(cpu, &cpu->B); break; }
                case SUB_C: { OP_SUB(cpu, &cpu->C); break; }
                case SUB_D: { OP_SUB(cpu, &cpu->D); break; }
                case SUB_E: { OP_SUB(cpu, &cpu->E); break; }
                case SUB_H: { OP_SUB(cpu, &cpu->H); break; }
                case SUB_L: { OP_SUB(cpu, &cpu->L); break; }
                case SUB_M: { OP_SUB(cpu, read_memp_HL(cpu)); break; }
                case SUB_A: { OP_SUB(cpu, &cpu->A); break; }

                case SUI: { OP_SUI(cpu); break; }
                case STA: { OP_STA(cpu); break; }
                case STAX_BC: { OP_STAX(cpu, read_memp_BC(cpu)); break; }
                case STAX_DE: { OP_STAX(cpu, read_memp_DE(cpu)); break; }

                case XCHG: { OP_XCHG(cpu); break; }
                case XTHL: { OP_XTHL(cpu); break; }

                case XRA_B: { OP_XRA(cpu, &cpu->B); break; }
                case XRA_C: { OP_XRA(cpu, &cpu->C); break; }
                case XRA_D: { OP_XRA(cpu, &cpu->D); break; }
                case XRA_E: { OP_XRA(cpu, &cpu->E); break; }
                case XRA_H: { OP_XRA(cpu, &cpu->H); break; }
                case XRA_L: { OP_XRA(cpu, &cpu->L); break; }
                case XRA_M: { OP_XRA(cpu, read_memp_HL(cpu)); break; }
                case XRA_A: { OP_XRA(cpu, &cpu->A); break; }

                case XRI: { OP_XRI(cpu); break; }

                default: { fprintf(stderr, "Unrecognized opcode %02X\n", op); exit(1); }
        }
}