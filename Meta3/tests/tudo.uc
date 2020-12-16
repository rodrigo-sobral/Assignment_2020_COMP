void v(void);
void g(void) {
    return v();
}

int main(void x) {
    if (2) return 1;

    if (1) {
        return 5;
    } else {
        return 9;
    }
}