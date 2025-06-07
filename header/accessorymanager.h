#ifndef ACCESSORYMANAGER_H
#define ACCESSORYMANAGER_H
// 이 파일이 여러 번 include되는 것을 방지하기 위해 #ifndef ~ #define ~ #endif 구조인 Include Guard 사용

#include <vector> // <vector>: std::vector 사용하기 위해 필요
#include <string> // <string>: std::string 사용을 위한 기본 헤더
#include "accessory.h" // "accessory.h": Accessory 클래스 정의가 필요하므로 포함

class AccessoryManager { // Accessory 객체들을 관리하기 위한 매니저 클래스 정의
private:
    std::vector<Accessory> accessories;
    // accessories: 액세서리 객체들을 저장할 동적 배열
    // 외부에서 직접 접근 불가 → 캡슐화 (encapsulation)

public:
    AccessoryManager();
    // 기본 생성자, 객체 생성 시 초기화 작업 담당 (예: 파일에서 데이터 불러오기)

    void loadFromFile(); // loadFromFile(): txt 파일에서 액세서리 목록 읽어와서 accessories 벡터에 저장
    void saveToFile() const; // saveToFile() const: 현재 메모리 상의 액세서리 목록을 파일로 저장 및 멤버 변수 수정 금지

    void inputAccessory();    // 콘솔 입력
    void deleteAccessory();   // 코드 기준 삭제
    void displayAccessory() const; // const: 객체 내부 상태를 바꾸지 않음

    const Accessory* findByCode(const std::string& code) const;
    Accessory* findByCode(const std::string& code);
    // 오버로딩된 두 함수:
    // const 버전은 읽기 전용 포인터 반환
    // 비-const 버전은 수정 가능한 포인터 반환
    // 특정 코드에 해당하는 액세서리 객체 포인터를 반환

    bool displayMenu();    // 루프 담당, true 반환은 상위 메뉴(예: ProductManager)로 돌아가기
};

#endif // ACCESSORYMANAGER_H