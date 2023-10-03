#ifndef CFBF_DIRECTORY_ENTRY_H
#define CFBF_DIRECTORY_ENTRY_H

#include <stdint.h>

// 디렉터리 엔트리 헤더 구조체 정의
#pragma pack(push, 1)  // 바이트 정렬을 1바이트로 설정

struct CFBFDirectoryEntryHeader {
    char name[64];             // 이름 
    uint16_t nameLength;       // 이름의 길이
    uint8_t objectType;       // 객체 형식 (0x05: 스트림, 0x02: 저장소)
    uint8_t colorFlag;         // color flag
    uint32_t leftSiblingID;    // 왼쪽 형제의 객체 ID
    uint32_t rightSiblingID;   // 오른쪽 형제의 객체 ID
    uint32_t childID;          // 자식 객체의 객체 ID
    uint8_t uniqueID[16];      // 고유 식별자 (클래스 ID)
    uint16_t stateBits;        // 상태 비트
    uint64_t creationTime;     // 생성 시간 (64비트 정수)
    uint64_t modifyTime;       // 수정 시간 (64비트 정수)
    uint32_t startingSector;   // 시작 섹터
    uint64_t streamSize;       // 스트림 크기 (64비트 정수)
};


#pragma pack(pop)  // 이전에 설정한 바이트 정렬 상태 복원

#endif /* CFBF_DIRECTORY_ENTRY_H */ 

