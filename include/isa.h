//
// Created by dev on 2/9/26.
//

#ifndef INC_8080_EMULATOR_ISA_H
#define INC_8080_EMULATOR_ISA_H

/**
 * Compact representation of the 8080 instruction set using base opcodes.
 *
 * This enum uses "base opcodes" for instruction groups where the full
 * 256-entry opcode map encodes registers or variants inside the opcode
 * byte. For those groups the comment shows the full opcode range and a
 * short decode hint (how to extract destination/source registers).
 *
 * Examples:
 *   - MOV: opcodes 0x40..0x7F encode MOV r_dest, r_src. Use
 *       dest = (opcode >> 3) & 0x7;
 *       src  = opcode & 0x7;
 *   - MVI/INR/DCR/ADD/ADC/... similar groups step by 8 for the 8 registers.
 */
typedef enum {
    // Move, Load, and Store
    ISA_MOV = 0x40,     /**< MOV r1, r2 - Base opcode for MOV block (opcodes 0x40-0x7F).
                                  Encoded: dest = (opcode >> 3) & 0x7, src = opcode & 0x7.
                              Example: 0x78 => MOV A,B (moves B into A). */
    ISA_MVI = 0x06,     /**< MVI r, byte - Base opcode for MVI (opcodes 0x06,0x0E,0x16,...,0x3E).
                              Encoded: reg = (opcode >> 3) & 0x7; step=0x08.
                              Example: 0x3E 0x05 => MVI A,0x05 (A := 0x05). */
    ISA_LDA = 0x3A,     /**< LDA addr - Load accumulator from memory address
                              Example: 0x3A 0x34 0x12 => LDA 0x1234  ; A := mem[0x1234] */
    ISA_STA = 0x32,     /**< STA addr - Store accumulator to memory address
                              Example: 0x32 0x78 0x56 => STA 0x5678 ; mem[0x5678] := A */
    ISA_LHLD = 0x2A,    /**< LHLD addr - Load H and L registers from memory
                              Example: 0x2A 0x00 0x10 => LHLD 0x1000 ; L:=mem[0x1000], H:=mem[0x1001] */
    ISA_SHLD = 0x22,    /**< SHLD addr - Store H and L registers to memory
                              Example: 0x22 0x00 0x20 => SHLD 0x2000 ; mem[0x2000]:=L, mem[0x2001]:=H */
    ISA_LDAX = 0x0A,    /**< LDAX rp - Load accumulator from address in register pair
                              Example: 0x0A (with BC=0x3000) => A := mem[BC] */
    ISA_STAX = 0x02,    /**< STAX rp - Store accumulator to address in register pair
                              Example: 0x02 (with BC=0x4000) => mem[BC] := A */

    // Arithmetic (many are register-encoded: base shown)
    ISA_ADD = 0x80,     /**< ADD r - Base for ADD r (opcodes 0x80-0x87). reg = opcode & 0x7
                              Example: 0x80 => ADD B  ; A := A + B (flags updated) */
    ISA_ADI = 0xC6,     /**< ADI byte - Add immediate to accumulator
                              Example: 0xC6 0x10 => ADI 0x10 ; A := A + 0x10 */
    ISA_ADC = 0x88,     /**< ADC r - Base for ADC r (opcodes 0x88-0x8F)
                              Example: 0x8F => ADC A ; A := A + A + CY */
    ISA_ACI = 0xCE,     /**< ACI byte - Add immediate to accumulator with carry
                              Example: 0xCE 0x01 => ACI 0x01 ; A := A + 0x01 + CY */
    ISA_SUB = 0x90,     /**< SUB r - Base for SUB r (opcodes 0x90-0x97)
                              Example: 0x90 => SUB B ; A := A - B */
    ISA_SUI = 0xD6,     /**< SUI byte - Subtract immediate from accumulator
                              Example: 0xD6 0x05 => SUI 0x05 ; A := A - 0x05 */
    ISA_SBB = 0x98,     /**< SBB r - Base for SBB r (opcodes 0x98-0x9F)
                              Example: 0x99 => SBB C ; A := A - C - CY */
    ISA_SBI = 0xDE,     /**< SBI byte - Subtract immediate from accumulator with borrow
                              Example: 0xDE 0x01 => SBI 0x01 ; A := A - 0x01 - CY */
    ISA_INR = 0x04,     /**< INR r - Base for INR r (opcodes 0x04,0x0C,...,0x3C). reg = (opcode>>3)&0x7
                              Example: 0x04 => INR B ; B := B + 1 (flags updated, CY unaffected) */
    ISA_DCR = 0x05,     /**< DCR r - Base for DCR r (opcodes 0x05,0x0D,...,0x3D).
                              Example: 0x0D => DCR C ; C := C - 1 */
    ISA_INX = 0x03,     /**< INX rp - Increment register pair (opcodes 0x03,0x13,0x23,0x33)
                              Example: 0x23 => INX H ; HL := HL + 1 */
    ISA_DCX = 0x0B,     /**< DCX rp - Decrement register pair (opcodes 0x0B,0x1B,0x2B,0x3B)
                              Example: 0x1B => DCX D ; DE := DE - 1 */
    ISA_DAD = 0x09,     /**< DAD rp - Add register pair to H and L (opcodes 0x09,0x19,0x29,0x39)
                              Example: 0x19 => DAD D ; HL := HL + DE (CY updated) */
    ISA_DAA = 0x27,     /**< DAA - Decimal adjust accumulator
                              Example: 0x27 => DAA ; adjust A for BCD after add/sub */

    // Logical
    ISA_ANA = 0xA0,     /**< ANA r - Base for ANA r (opcodes 0xA0-0xA7)
                              Example: 0xA1 => ANA C ; A := A & C */
    ISA_ANI = 0xE6,     /**< ANI byte - AND immediate with accumulator
                              Example: 0xE6 0x0F => ANI 0x0F ; A := A & 0x0F */
    ISA_ORA = 0xB0,     /**< ORA r - Base for ORA r (opcodes 0xB0-0xB7)
                              Example: 0xB7 => ORA A ; A := A | A */
    ISA_ORI = 0xF6,     /**< ORI byte - OR immediate with accumulator
                              Example: 0xF6 0xFF => ORI 0xFF ; A := A | 0xFF */
    ISA_XRA = 0xA8,     /**< XRA r - Base for XRA r (opcodes 0xA8-0xAF)
                              Example: 0xAF => XRA A ; A := A ^ A (sets Z) */
    ISA_XRI = 0xEE,     /**< XRI byte - XOR immediate with accumulator
                              Example: 0xEE 0x0F => XRI 0x0F */
    ISA_CMP = 0xB8,     /**< CMP r - Base for CMP r (opcodes 0xB8-0xBF)
                              Example: 0xB8 => CMP B ; compare A and B (flags set) */
    ISA_CPI = 0xFE,     /**< CPI byte - Compare immediate with accumulator
                              Example: 0xFE 0x01 => CPI 0x01 ; compare A with 0x01 */
    ISA_RLC = 0x07,     /**< RLC - Rotate accumulator left
                              Example: 0x07 => RLC ; A rotated left (bit7->bit0, bit7->CY) */
    ISA_RRC = 0x0F,     /**< RRC - Rotate accumulator right
                              Example: 0x0F => RRC ; A rotated right */
    ISA_RAL = 0x17,     /**< RAL - Rotate accumulator left through carry
                              Example: 0x17 => RAL ; A<<1 with old CY->bit0 */
    ISA_RAR = 0x1F,     /**< RAR - Rotate accumulator right through carry
                              Example: 0x1F => RAR */
    ISA_CMA = 0x2F,     /**< CMA - Complement accumulator
                              Example: 0x2F => CMA ; A := ~A */
    ISA_CMC = 0x3F,     /**< CMC - Complement carry flag
                              Example: 0x3F => CMC ; CY := !CY */
    ISA_STC = 0x37,     /**< STC - Set carry flag
                              Example: 0x37 => STC ; CY := 1 */

    // Branch
    ISA_JMP = 0xC3,     /**< JMP addr - Unconditional jump
                              Example: 0xC3 0x00 0x10 => JMP 0x1000 ; PC := 0x1000 */
    ISA_JZ = 0xCA,      /**< JZ addr - Jump if zero
                              Example: 0xCA 0x10 0x00 => JZ 0x0010 (jump if Z==1) */
    ISA_JNZ = 0xC2,     /**< JNZ addr - Jump if not zero
                              Example: 0xC2 0x20 0x00 => JNZ 0x0020 (jump if Z==0) */
    ISA_JC = 0xDA,      /**< JC addr - Jump if carry
                              Example: 0xDA 0x30 0x00 => JC 0x0030 (jump if CY==1) */
    ISA_JNC = 0xD2,     /**< JNC addr - Jump if no carry
                              Example: 0xD2 0x40 0x00 => JNC 0x0040 (jump if CY==0) */
    ISA_JP = 0xF2,      /**< JP addr - Jump if positive
                              Example: 0xF2 0x00 0x80 => JP 0x8000 (jump if sign==0) */
    ISA_JM = 0xFA,      /**< JM addr - Jump if minus
                              Example: 0xFA 0xFF 0x7F => JM 0x7FFF (jump if sign==1) */
    ISA_JPE = 0xEA,     /**< JPE addr - Jump if parity even
                              Example: 0xEA 0x00 0x20 => JPE 0x2000 (jump if parity even) */
    ISA_JPO = 0xE2,     /**< JPO addr - Jump if parity odd
                              Example: 0xE2 0x00 0x30 => JPO 0x3000 */
    ISA_CALL = 0xCD,    /**< CALL addr - Call subroutine
                              Example: 0xCD 0x00 0x50 => CALL 0x5000 (push PC; PC:=0x5000) */
    ISA_CZ = 0xCC,      /**< CZ addr - Call if zero
                              Example: 0xCC 0x00 0x60 => CZ 0x6000 (if Z==1) */
    ISA_CNZ = 0xC4,     /**< CNZ addr - Call if not zero
                              Example: 0xC4 0x00 0x70 => CNZ 0x7000 (if Z==0) */
    ISA_CC = 0xDC,      /**< CC addr - Call if carry
                              Example: 0xDC 0x00 0x80 => CC 0x8000 (if CY==1) */
    ISA_CNC = 0xD4,     /**< CNC addr - Call if no carry
                              Example: 0xD4 0x00 0x90 => CNC 0x9000 (if CY==0) */
    ISA_CP = 0xF4,      /**< CP addr - Call if positive
                              Example: 0xF4 0x00 0xA0 => CP 0xA000 (if sign==0) */
    ISA_CM = 0xFC,      /**< CM addr - Call if minus
                              Example: 0xFC 0x00 0xB0 => CM 0xB000 (if sign==1) */
    ISA_CPE = 0xEC,     /**< CPE addr - Call if parity even
                              Example: 0xEC 0x00 0xC0 => CPE 0xC000 */
    ISA_CPO = 0xE4,     /**< CPO addr - Call if parity odd
                              Example: 0xE4 0x00 0xD0 => CPO 0xD000 */
    ISA_RET = 0xC9,     /**< RET - Return from subroutine
                              Example: 0xC9 => RET ; PC := pop() */
    ISA_RZ = 0xC8,      /**< RZ - Return if zero
                              Example: 0xC8 => RZ ; if Z==1 PC := pop() */
    ISA_RNZ = 0xC0,     /**< RNZ - Return if not zero
                              Example: 0xC0 => RNZ ; if Z==0 PC := pop() */
    ISA_RC = 0xD8,      /**< RC - Return if carry
                              Example: 0xD8 => RC ; if CY==1 PC := pop() */
    ISA_RNC = 0xD0,     /**< RNC - Return if no carry
                              Example: 0xD0 => RNC ; if CY==0 PC := pop() */
    ISA_RP = 0xF0,      /**< RP - Return if positive
                              Example: 0xF0 => RP ; if sign==0 PC := pop() */
    ISA_RM = 0xF8,      /**< RM - Return if minus
                              Example: 0xF8 => RM ; if sign==1 PC := pop() */
    ISA_RPE = 0xE8,     /**< RPE - Return if parity even
                              Example: 0xE8 => RPE ; if parity even PC := pop() */
    ISA_RPO = 0xE0,     /**< RPO - Return if parity odd
                              Example: 0xE0 => RPO ; if parity odd PC := pop() */
    ISA_RST = 0xC7,     /**< RST n - Base for restart block. Full opcodes: 0xC7,0xCF,0xD7,0xDF,0xE7,0xEF,0xF7,0xFF
                              Encoded: rst_vector = (opcode >> 3) & 0x7
                              Example: 0xCF => RST 1 ; push PC and jump to 0x08 * 1 = 0x08 */
    // Stack
    ISA_PUSH = 0xC5,    /**< PUSH rp - Base for PUSH (0xC5,0xD5,0xE5,0xF5)
                              Example: 0xC5 => PUSH B (push BC to stack) */
    ISA_POP = 0xC1,     /**< POP rp - Base for POP (0xC1,0xD1,0xE1,0xF1)
                              Example: 0xD1 => POP D (pop into DE) */
    ISA_XTHL = 0xE3,    /**< XTHL - Exchange H:L with top of stack
                              Example: 0xE3 => XTHL ; swap HL with (SP) */
    ISA_SPHL = 0xF9,    /**< SPHL - Move H:L to stack pointer
                              Example: 0xF9 => SPHL ; SP := HL */

    // I/O
    ISA_IN = 0xDB,      /**< IN port - Input from port
                              Example: 0xDB 0x05 => IN 0x05 ; A := IN(0x05) */
    ISA_OUT = 0xD3,     /**< OUT port - Output to port
                              Example: 0xD3 0x05 => OUT 0x05 ; OUT(0x05) := A */

    // Control
    ISA_HLT = 0x76,     /**< HLT - Halt
                              Example: 0x76 => HLT ; stop CPU until interrupt */
    ISA_NOP = 0x00,     /**< NOP - No operation
                              Example: 0x00 => NOP */
    ISA_EI = 0xFB,      /**< EI - Enable interrupts
                              Example: 0xFB => EI ; interrupts enabled */
    ISA_DI = 0xF3,      /**< DI - Disable interrupts
                              Example: 0xF3 => DI ; interrupts disabled */

    // Misc
    ISA_XCHG = 0xEB,    /**< XCHG - Exchange H:L with D:E
                              Example: 0xEB => XCHG ; HL <-> DE */
    ISA_PCHL = 0xE9     /**< PCHL - Jump to address in H:L
                              Example: 0xE9 => PCHL ; PC := HL */
} isa_t;

#endif //INC_8080_EMULATOR_ISA_H

