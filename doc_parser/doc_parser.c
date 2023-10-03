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

        // 스토리지 또는 스트림 정보 출력
        if (entryHeader.objectType == 0x02) {
            printf("스토리지: ");
            for (int i = 0; i < entryHeader.nameLength; i=i+2) {
            printf("%c", (unsigned char)entryHeader.name[i]);
            }
            printf("\n");
        } else if (entryHeader.objectType == 0x01) {
            printf("스트림: ");
            for (int i = 0; i < entryHeader.nameLength*2; i=i+2) {
            printf("%c", (unsigned char)entryHeader.name[i]);
            }
            printf("\n");
        } else if (entryHeader.objectType == 0x05){
            printf("\n------------스토리지 정보------------\n");
            printf("\n");
        }

        // 다음 디렉터리 엔트리 위치 확인
        if (entryHeader.objectType == 0x00 || feof(file)) {
            break; // 디렉터리 엔트리가 없거나 파일의 끝이면 루프 종료
        }
        cnt++;
        fseek(file, offset + cnt*0x80, SEEK_SET);
    }
}

void parseCFBFHeader(FILE* file) {
    struct CFBFHeader header;

    // CFBF 파일 헤더를 읽어옵니다.
    if (fread(&header, sizeof(struct CFBFHeader), 1, file) != 1) {
        perror("헤더 읽기 오류");
        return;
    }

    // 헤더 정보 출력
    printf("\n------------헤더 정보------------\n\n");
    printf("시그니처: ");
    for (int i = 0; i < 8; i++) {
        printf("%02X ", header.signature[i]);
        }
    printf("\n");

    printf("마이너 버전: %02X\n", header.minorVersion);
    printf("메이저 버전: %02X\n", header.majorVersion);
    printf("바이트 순서: %02X\n", header.byteOrder);
    printf("섹터 쉬프트: %02X\n", header.sectorShift);
    printf("미니 섹터 쉬프트: %02X\n", header.miniSectorShift);
    printf("디렉터리 섹터 수: %02X\n", header.directorySectors);
    printf("FAT 섹터 수: %02X\n", header.fatSectors);
    printf("첫 번째 디렉터리 섹터 위치: %02X\n", header.firstDirectorySectorLocation);
    printf("트랜잭션 시그니처 번호: %02X\n", header.transactionSignatureNumber);
    printf("미니 스트림 절단 크기: %02X\n", header.miniStreamCutoffSize);
    printf("첫 번째 미니 FAT 섹터 위치: %02X\n", header.firstMiniFATSectorLocation);
    printf("미니 FAT 섹터 수: %02X\n", header.miniFATSectors);
    printf("첫 번째 DIFAT 섹터 위치: %02X\n", header.firstDIFATSectorLocation);
    printf("DIFAT 섹터 수: %02X\n", header.DIFATSectors);

    unsigned int rawAddr = (header.firstDirectorySectorLocation + 1 ) * pow(2,header.sectorShift);
    printf("\n루트 엔트리 주소: 0X%02X\n",rawAddr);

    fseek(file, rawAddr, SEEK_SET);
    parseCFBFDirectoryEntry(file, rawAddr);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("사용법: %s <CFBF 파일 경로>\n", argv[0]);
        return 1; // 명령 프롬프트에서 파일 경로를 입력받지 않으면 오류 반환
    }

    const char* filePath = argv[1];
    FILE* file = fopen(filePath, "rb");

    if (!file) {
        perror("파일 열기 오류");
        return 1; // 파일 열기 오류 반환
    }

    unsigned char byte[8] = "";
    fread(byte, 8, 1, file);
    
    printf("\n");

    if (byte[0] == 0xD0 && byte[1] == 0xCF && byte[2] == 0x11) {
        printf("CFBF 포맷을 파싱합니다.\n");
        rewind(file); // 커서를 맨 앞으로 조정
        parseCFBFHeader(file); // CFBF 포맷 헤더 파싱
    } else {
        printf("지원하지 않는 파일 형식입니다.\n");
    }

    fclose(file);

    return 0; // 프로그램 종료 (성공)
}