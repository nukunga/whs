#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "cfbf_header.h"
#include "cfbf_directory_entry.h"

void parseCFBFDirectoryEntry(FILE* file, long int offset) {
    int cnt = 0;
    
    while (1) {
        struct CFBFDirectoryEntryHeader entryHeader;
        fread(&entryHeader, sizeof(struct CFBFDirectoryEntryHeader), 1, file);

        // ���丮�� �Ǵ� ��Ʈ�� ���� ���
        if (entryHeader.objectType == 0x02) {
            printf("���丮��: ");
            for (int i = 0; i < entryHeader.nameLength; i=i+2) {
            printf("%c", (unsigned char)entryHeader.name[i]);
            }
            printf("\n");
        } else if (entryHeader.objectType == 0x01) {
            printf("��Ʈ��: ");
            for (int i = 0; i < entryHeader.nameLength*2; i=i+2) {
            printf("%c", (unsigned char)entryHeader.name[i]);
            }
            printf("\n");
        } else if (entryHeader.objectType == 0x05){
            printf("\n------------���丮�� ����------------\n");
            printf("\n");
        }

        // ���� ���͸� ��Ʈ�� ��ġ Ȯ��
        if (entryHeader.objectType == 0x00 || feof(file)) {
            break; // ���͸� ��Ʈ���� ���ų� ������ ���̸� ���� ����
        }
        cnt++;
        fseek(file, offset + cnt*0x80, SEEK_SET);
    }
}

void parseCFBFHeader(FILE* file) {
    struct CFBFHeader header;

    // CFBF ���� ����� �о�ɴϴ�.
    if (fread(&header, sizeof(struct CFBFHeader), 1, file) != 1) {
        perror("��� �б� ����");
        return;
    }

    // ��� ���� ���
    printf("\n------------��� ����------------\n\n");
    printf("�ñ״�ó: ");
    for (int i = 0; i < 8; i++) {
        printf("%02X ", header.signature[i]);
        }
    printf("\n");

    printf("���̳� ����: %02X\n", header.minorVersion);
    printf("������ ����: %02X\n", header.majorVersion);
    printf("����Ʈ ����: %02X\n", header.byteOrder);
    printf("���� ����Ʈ: %02X\n", header.sectorShift);
    printf("�̴� ���� ����Ʈ: %02X\n", header.miniSectorShift);
    printf("���͸� ���� ��: %02X\n", header.directorySectors);
    printf("FAT ���� ��: %02X\n", header.fatSectors);
    printf("ù ��° ���͸� ���� ��ġ: %02X\n", header.firstDirectorySectorLocation);
    printf("Ʈ����� �ñ״�ó ��ȣ: %02X\n", header.transactionSignatureNumber);
    printf("�̴� ��Ʈ�� ���� ũ��: %02X\n", header.miniStreamCutoffSize);
    printf("ù ��° �̴� FAT ���� ��ġ: %02X\n", header.firstMiniFATSectorLocation);
    printf("�̴� FAT ���� ��: %02X\n", header.miniFATSectors);
    printf("ù ��° DIFAT ���� ��ġ: %02X\n", header.firstDIFATSectorLocation);
    printf("DIFAT ���� ��: %02X\n", header.DIFATSectors);

    unsigned int rawAddr = (header.firstDirectorySectorLocation + 1 ) * pow(2,header.sectorShift);
    printf("\n��Ʈ ��Ʈ�� �ּ�: 0X%02X\n",rawAddr);

    fseek(file, rawAddr, SEEK_SET);
    parseCFBFDirectoryEntry(file, rawAddr);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("����: %s <CFBF ���� ���>\n", argv[0]);
        return 1; // ��� ������Ʈ���� ���� ��θ� �Է¹��� ������ ���� ��ȯ
    }

    const char* filePath = argv[1];
    FILE* file = fopen(filePath, "rb");

    if (!file) {
        perror("���� ���� ����");
        return 1; // ���� ���� ���� ��ȯ
    }

    unsigned char byte[8] = "";
    fread(byte, 8, 1, file);
    
    printf("\n");

    if (byte[0] == 0xD0 && byte[1] == 0xCF && byte[2] == 0x11) {
        printf("CFBF ������ �Ľ��մϴ�.\n");
        rewind(file); // Ŀ���� �� ������ ����
        parseCFBFHeader(file); // CFBF ���� ��� �Ľ�
    } else {
        printf("�������� �ʴ� ���� �����Դϴ�.\n");
    }

    fclose(file);

    return 0; // ���α׷� ���� (����)
}