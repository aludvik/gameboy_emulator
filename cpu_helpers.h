// Endian
void rpw(reg *r1, reg *r2, word value) {
    *r1 = (value & 0xFF00) >> 8;
    *r2 = value & 0x00FF;
}

word rpr(reg r1, reg r2) {
    return r1 + (r2 << 8);
}

void inc_r(reg *r) {
    *r = (*r + 1) & 0xFF; 
}

void dec_r(reg *r) {
    *r = (*r - 1) & 0xFF; 
}

void ld_r_r(reg *r1, reg *r2) {
    *r1 = *r2;
}

CPUUpdate ld_r_prr(reg *ld, reg *r1, reg *r2) {
    CPUUpdate u = {1, 8};
    *ld = rb(rpr(*r1, *r2));
    return u;
}

CPUUpdate ld_r_d8(reg *r) {
    CPUUpdate u = {2, 8};
    *r = rb(cpu.pc + 1);
    return u;
}

CPUUpdate ld_rr_d16(reg *r1, reg *r2) {
    CPUUpdate u = {3, 12};
    rpw(r1, r2, rw((cpu.pc) + 1));
    return u;
}

CPUUpdate ld_prr_A(reg *r1, reg *r2) {
    CPUUpdate u = {1, 8};
    wb(rpr(*r1, *r2), cpu.a);
    return u;
}
