struct gba_rom * read_rom(u8 *filename){
    FILE * f;
    struct gba_rom *gamerom;
    gamerom = malloc(sizeof(struct gba_rom));
    f = fopen(filename, "rw");
    if(f == NULL){
        printf("file open failed\n");
    }
    fseek(f, 0, SEEK_END);
    gamerom->FileSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    printf("gba file size : %ld byte\n", gamerom->FileSize);
    gamerom->gba_rom_buf = malloc(gamerom->FileSize + 1);
    printf("file buffer malloc success\n");

    fread(gamerom->gba_rom_buf, gamerom->FileSize, 1, f);

    fclose(f);
    f = NULL;
    return gamerom;
}