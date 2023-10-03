#ifndef CFBF_HEADER_H
#define CFBF_HEADER_H

#include <stdint.h>

// CFBF 헤더 구조체 정의
#pragma pack(push, 1)  // 바이트 정렬을 1바이트로 설정

struct CFBFHeader {
    uint8_t signature[8];       // 시그니처 ("D0CF11E0A1B11AE1")
    uint16_t clsid[8];          // 클래스 ID
    uint16_t minorVersion;      // 마이너 버전
    uint16_t majorVersion;      // 메이저 버전
    uint16_t byteOrder;         // 바이트 순서 (0xFFFE: Little-endian, 0xFEFF: Big-endian)
    uint16_t sectorShift;       // 섹터 크기 쉬프트
    uint16_t miniSectorShift;   // 미니 섹터 크기 쉬프트
    uint8_t reserved1[6];      // 예약된 필드
    uint32_t directorySectors;  // 디렉터리 섹터 수
    uint32_t fatSectors;        // FAT (File Allocation Table) 섹터 수
    uint32_t firstDirectorySectorLocation;  // 첫 번째 디렉터리 섹터 위치
    uint32_t transactionSignatureNumber;    // 트랜잭션 시그니처 번호
    uint32_t miniStreamCutoffSize;          // 미니 스트림 절단 크기
    uint32_t firstMiniFATSectorLocation;    // 첫 번째 미니 FAT 섹터 위치
    uint32_t miniFATSectors;                // 미니 FAT 섹터 수
    uint32_t firstDIFATSectorLocation;      // 첫 번째 DIFAT 섹터 위치
    uint32_t DIFATSectors;                 // DIFAT 섹터 수
    uint32_t reserved2[4];                // 예약된 필드
};

#pragma pack(pop)  // 이전에 설정한 바이트 정렬 상태 복원

#endif /* CFBF_HEADER_H */