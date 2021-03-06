/*
 *  Copyright (C) 2004-2009 The libbeauty Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <rev.h>
#include "internal.h"

int prefix_0f(void *handle_void, struct dis_instructions_s *dis_instructions, uint8_t *base_address, uint64_t offset, uint64_t size, uint8_t rex) {
	int half;
	uint8_t reg = 0;
	int tmp;
	int result = 0;
	uint8_t byte;
	//int8_t rel8 = 0;
	int32_t rel32 = 0;
	int64_t rel64 = 0;
	struct instruction_s *instruction;
	byte = base_address[offset + dis_instructions->bytes_used++];
	debug_print(DEBUG_INPUT_DIS, 1, "Prefix_0f Byte = 0x%x\n", byte);
	switch (byte) {
	case 0x00:												/* GRP 6 Exxx */
	case 0x01:												/* Group 7 Ev */
	case 0x02:												/* LAR Gv,Ev */
	case 0x03:												/* LSL Gv,Ev */
	case 0x06:												/* CLTS */
		break;
	case 0x1F:												/* NOP */
		byte = base_address[offset + dis_instructions->bytes_used++]; 
		switch (byte) {
		case 0x84:
			dis_instructions->bytes_used++; 
		case 0x80:
			dis_instructions->bytes_used += 2; 
		case 0x44:
			dis_instructions->bytes_used++; 
		case 0x40:
			dis_instructions->bytes_used++; 
		case 0x00: 
			result = 1;
			break;
		default:
			break;
		}
		break;
	case 0x20:												/* MOV Rd.CRx */
		break;
	case 0x21:												/* MOV Rd,DRx */
		break;
	case 0x22:												/* MOV CRx,Rd */
		break;
	case 0x23:												/* MOV DRx,Rd */
		break;
	/* CMOVcc */
	case 0x40:												/* CMOVO */
	case 0x41:												/* CMOVNO */
	case 0x42:												/* CMOVB */
	case 0x43:												/* CMOVNB */
	case 0x44:												/* CMOVZ */
	case 0x45:												/* CMOVNZ */
	case 0x46:												/* CMOVBE */
	case 0x47:												/* CMOVNBE */
	case 0x48:												/* CMOVS */
	case 0x49:												/* CMOVNS */
	case 0x4a:												/* CMOVP */
	case 0x4b:												/* CMOVNP */
	case 0x4c:												/* CMOVL */
	case 0x4d:												/* CMOVNL */
	case 0x4e:												/* CMOVLE */
	case 0x4f:												/* CMOVNLE */
		/* FIXME: Add the MOV bit */
		instruction = &dis_instructions->instruction[dis_instructions->instruction_number];	
		instruction->opcode = IF;
		instruction->flags = 0;
		instruction->dstA.store = STORE_DIRECT;
		instruction->dstA.indirect = IND_DIRECT;
		instruction->dstA.indirect_size = 64;
		/* Means get from rest of instruction */
		//relative = getbyte(base_address, offset + dis_instructions->bytes_used);
		/* extends byte to int64_t */
		rel64 = 0; /* Skip to next instruction */
		instruction->dstA.index = rel64;
		instruction->dstA.relocated = 0;
		instruction->dstA.value_size = 32;
		instruction->srcA.store = STORE_DIRECT;
		instruction->srcA.indirect = IND_DIRECT;
		instruction->srcA.indirect_size = 64;
		instruction->srcA.index = ((byte & 0xf) ^ 0x01) + 1; /* CONDITION to skip mov instruction */
		instruction->srcA.relocated = 0;
		instruction->srcA.value_size = 32;
		debug_print(DEBUG_INPUT_DIS, 1, "JCD7: Before: %d\n", dis_instructions->instruction_number);
		dis_instructions->instruction_number++;
		result = dis_Gx_Ex(handle_void, MOV, rex, dis_instructions, base_address, offset, &reg, size);
		debug_print(DEBUG_INPUT_DIS, 1, "JCD7: After: %d\n", dis_instructions->instruction_number);
		break;
	/* JMP */
	case 0x80:												/* JO */
	case 0x81:												/* JNO */
	case 0x82:												/* JB */
	case 0x83:												/* JNB */
	case 0x84:												/* JZ */
	case 0x85:												/* JNZ */
	case 0x86:												/* JBE */
	case 0x87:												/* JNBE */
	case 0x88:												/* JS */
	case 0x89:												/* JNS */
	case 0x8a:												/* JP */
	case 0x8b:												/* JNP */
	case 0x8c:												/* JL */
	case 0x8d:												/* JNL */
	case 0x8e:												/* JLE */
	case 0x8f:												/* JNLE */
		instruction = &dis_instructions->instruction[dis_instructions->instruction_number];	
		instruction->opcode = IF;
		instruction->flags = 0;
		instruction->dstA.store = STORE_DIRECT;
		instruction->dstA.indirect = IND_DIRECT;
		instruction->dstA.indirect_size = 64;
		/* Means get from rest of instruction */
		rel32 = getdword(base_address, offset + dis_instructions->bytes_used);
		rel64 = rel32;
		dis_instructions->bytes_used += 4; 
		/* extends byte to int64_t */
		instruction->dstA.index = rel64;
		instruction->dstA.relocated = 0;
		instruction->dstA.value_size = 32;
		instruction->srcA.store = STORE_DIRECT;
		instruction->srcA.indirect = IND_DIRECT;
		instruction->srcA.indirect_size = 64;
		instruction->srcA.index = (byte & 0xf) + 1; /* CONDITION */
		instruction->srcA.relocated = 0;
		instruction->srcA.value_size = 32;
		dis_instructions->instruction_number++;
		result = 1;
		break;
	/* SET */
	case 0x90:												/* SETO */
	case 0x91:												/* SETNO */
	case 0x92:												/* SETB */
	case 0x93:												/* SETNB */
	case 0x94:												/* SETZ */
	case 0x95:												/* SETNZ */
	case 0x96:												/* SETBE */
	case 0x97:												/* SETNBE */
	case 0x98:												/* SETS */
	case 0x99:												/* SETNS */
	case 0x9a:												/* SETP */
	case 0x9b:												/* SETNP */
	case 0x9c:												/* SETL */
	case 0x9d:												/* SETNL */
	case 0x9e:												/* SETLE */
	case 0x9f:												/* SETNLE */
		instruction = &dis_instructions->instruction[dis_instructions->instruction_number];	
		instruction->opcode = IF;
		instruction->flags = 0;
		instruction->dstA.store = STORE_DIRECT;
		instruction->dstA.indirect = IND_DIRECT;
		instruction->dstA.indirect_size = 64;
		/* Means get from rest of instruction */
		//relative = getbyte(base_address, offset + dis_instructions->bytes_used);
		/* extends byte to int64_t */
		rel64 = 0; /* Skip to next instruction */
		instruction->dstA.index = rel64;
		instruction->dstA.relocated = 0;
		instruction->dstA.value_size = 32;
		instruction->srcA.store = STORE_DIRECT;
		instruction->srcA.indirect = IND_DIRECT;
		instruction->srcA.indirect_size = 64;
		instruction->srcA.index = ((byte & 0xf) ^ 0x01) + 1; /* CONDITION to skip mov instruction */
		instruction->srcA.relocated = 0;
		instruction->srcA.value_size = 32;
		dis_instructions->instruction_number++;
		tmp = rmb(handle_void, dis_instructions, base_address, offset, size, rex, &reg, &half);
		instruction = &dis_instructions->instruction[dis_instructions->instruction_number];	
		instruction->opcode = MOV;
		instruction->flags = 0;
		instruction->srcA.store = STORE_DIRECT;
		instruction->srcA.indirect = IND_DIRECT;
		instruction->srcA.indirect_size = 64;
		instruction->srcA.index = 1;
		instruction->srcA.relocated = 0;
		instruction->srcA.value_size = 8;
		instruction->dstA.store = STORE_REG;
		instruction->dstA.indirect = IND_DIRECT;
		instruction->dstA.indirect_size = 64;
		instruction->dstA.index = REG_TMP1;
		instruction->dstA.relocated = 0;
		instruction->dstA.value_size = size;
		dis_instructions->instruction_number++;
		result = 1;
		break;
	case 0xa0:												/* PUSH FS */		
	case 0xa1:												/* POP FS */		
	case 0xa2:												/* CPUID */
	case 0xa3:												/* BT Ev,Gv */
	case 0xa4:												/* SHLD Ev,Gv,Ib */
	case 0xa5:												/* SHLD Ev,Gv,CL */
	case 0xa8:												/* PUSH GS */		
	case 0xa9:												/* POP GS */		
	case 0xab:												/* BTS Ev,Gv */
	case 0xac:												/* SHRD Ev,Gv,Ib */
		break;
	case 0xad:												/* SHRD Ev,Gv,CL */
		break;
	case 0xaf:												/* IMUL Gv,Ev */
		break;
	case 0xb2:												/* LSS Ev */
	case 0xb3:												/* BTR Ev,Gv */
	case 0xb4:												/* LFS Ev */
	case 0xb5:												/* LGS Ev */
	case 0xb6:												/* MOVZX Gv,Eb */
		tmp = rmb(handle_void, dis_instructions, base_address, offset, size, rex, &reg, &half);
		instruction = &dis_instructions->instruction[dis_instructions->instruction_number];	
		instruction->opcode = MOV;
		instruction->flags = 0;
		instruction->dstA.store = STORE_REG;
		instruction->dstA.indirect = 0;
		instruction->dstA.indirect_size = 64;
		instruction->dstA.index = reg_table[reg].offset;
		instruction->dstA.value_size = size;
		if (!half) {
			instruction->srcA.store = STORE_REG;
			instruction->srcA.indirect = 1;
			instruction->srcA.indirect_size = 64;
			instruction->srcA.index = REG_TMP1;
		}
		instruction->srcA.relocated = 0;
		instruction->srcA.value_size = 8;
		dis_instructions->instruction_number++;
		result = 1;
		break;
	case 0xb7:												/* MOVZX Gv,Ev */
		tmp = rmb(handle_void, dis_instructions, base_address, offset, size, rex, &reg, &half);
		instruction = &dis_instructions->instruction[dis_instructions->instruction_number];	
		instruction->opcode = MOV;
		instruction->flags = 0;
		instruction->dstA.store = STORE_REG;
		instruction->dstA.indirect = 0;
		instruction->dstA.indirect_size = 64;
		instruction->dstA.index = reg_table[reg].offset;
		instruction->dstA.value_size = size;
		if (!half) {
			instruction->srcA.store = STORE_REG;
			instruction->srcA.indirect = 1;
			instruction->srcA.indirect_size = 64;
			instruction->srcA.index = REG_TMP1;
		}
		instruction->srcA.relocated = 0;
		instruction->srcA.value_size = 16;
		dis_instructions->instruction_number++;
		result = 1;
		break;


	case 0xbf:												/* MOVSX Gv,Ev */
	case 0xba:												/* GRP8 Ev,Ib */
	case 0xbb:												/* BTC Ev,Gv */
	case 0xbc:												/* BSF Gv,Ev */
	case 0xbd:												/* BSR Gv,Ev */
	case 0xbe:												/* MOVSX Gv,Eb */
	case 0xc8:												/* BSWAP EAX */
	case 0xc9:												/* BSWAP ECX */
	case 0xca:												/* BSWAP EDX */
	case 0xcb:												/* BSWAP EBX */
	case 0xcc:												/* BSWAP ESP */
	case 0xcd:												/* BSWAP EBP */
	case 0xce:												/* BSWAP ESI */
	case 0xcf:												/* BSWAP EDI */
		break;
	default:
		break;

	};
	return result;
}
		
