// Endian
void wrp(reg *r1, reg *r2, word value) {
    *r1 = (value & 0xFF00) >> 8;
    *r2 = value & 0x00FF;
}

word rrp(reg r1, reg r2) {
    return r1 + (r2 << 8);
}

void inc_r(reg *r) {
    *r = (*r + 1) & 0xFF; 
}


void dec_r(reg *r) {
    *r = (*r - 1) & 0xFF; 
}

CPUUpdate inc_r2(reg *r) {
    *r = (*r + 1) & 0xFFFF; 
    return (CPUUpdate) {1, 8};
}

CPUUpdate dec_r2(reg *r) {
    *r = (*r - 1) & 0xFFFF; 
    return (CPUUpdate) {1, 8};
}

CPUUpdate inc_rr(reg *r1, reg *r2) {
    *r2 = (*r2 + 1) & 0xFF;
    if (!*r2) *r1 = (*r1 + 1) & 0xFF;
    return (CPUUpdate) {1, 8};
}

CPUUpdate dec_rr(reg *r1, reg *r2) {
    if (!*r2) *r1 = (*r1 - 1) & 0xFF;
    *r2 = (*r2 - 1) & 0xFF;
    return (CPUUpdate) {1, 8};
}

CPUUpdate add_rr_r2(reg *r11, reg *r12, reg *r2) {
    byte sum;
    sum = rrp(*r11, *r12) + *r2;
    wrp(r11, r12, sum);
    return (CPUUpdate) {1, 8};
}

CPUUpdate add_rr_rr(reg *r11, reg *r12, reg *r21, reg *r22) {
    CPUUpdate u = {1, 8};
    byte sum;
    sum = rrp(*r11, *r12) + rrp(*r21, *r22);
    wrp(r11, r12, sum);
    return u;
}

void ld_r_r(reg *r1, reg *r2) {
    *r1 = *r2;
}

CPUUpdate ld_prr_r(reg *r11, reg *r12, reg *r2) {
    wb(rrp(*r11, *r12), *r2);
    return (CPUUpdate) {1, 8};
}

CPUUpdate ld_r_prr(reg *ld, reg *r1, reg *r2) {
    CPUUpdate u = {1, 8};
    *ld = rb(rrp(*r1, *r2));
    return u;
}

CPUUpdate ld_r_d8(reg *r) {
    CPUUpdate u = {2, 8};
    *r = rb(cpu.pc + 1);
    return u;
}

CPUUpdate ld_rr_d16(reg *r1, reg *r2) {
    wrp(r1, r2, rw((cpu.pc) + 1));
    return (CPUUpdate) {3, 12};
}

CPUUpdate ld_r2_d16(reg *r) {
    *r = rw((cpu.pc) + 1);
    return (CPUUpdate) {3, 12};
}

CPUUpdate ld_prr_r(reg *r11, reg *r12, reg *r2) {
    CPUUpdate u = {1, 8};
    wb(rrp(*r11, *r12), *r2);
    return u;
}

CPUUpdate jr(reg *pc) {
    // Set op length to 0 so pc isn't moved after
    CPUUpdate u = {0, 12};
    byte jump = rb(*pc + 1);
    if (jump & 0x80) { // Negative
        jump = -((~jump + 1) & 0xFF);
    }
    *pc += jump;
    return (CPUUpdate) {0, 12};
}

CPUUpdate jr_f(reg *pc, int flag) {
    if (flag) {
        return jr(&cpu.pc);
    } else {
        return (CPUUpdate) {2, 8};
    }
}
