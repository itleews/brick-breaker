# 🧱 MFC 벽돌깨기 게임 (Brick Breaker Game)

Microsoft Foundation Classes (MFC)를 사용하여 개발한 윈도우 데스크탑용 벽돌깨기 게임입니다.  
플레이어는 패들을 조작해 공을 튕기고, 화면 위의 벽돌을 모두 제거하면 클리어됩니다.

---

## 📌 주요 기능

- MFC 기반 윈도우 데스크탑 애플리케이션
- 공, 패들, 벽돌 충돌 감지 및 반사 처리
- 벽돌 제거 및 점수 시스템 (선택사항)
- 게임 오버 및 재시작 기능
- 깔끔한 UI 구성

---

## 🛠️ 개발 환경

- **언어**: C++
- **프레임워크**: MFC (Microsoft Foundation Classes)
- **개발 툴**: Visual Studio 2019 이상
- **대상 플랫폼**: Windows

---

## 🚀 실행 방법

1. Visual Studio에서 `BrickBreaker.sln` 열기
2. `Release` 또는 `Debug` 모드 선택
3. 빌드 (`Ctrl + Shift + B`) 후 실행 (`F5`)

---

## 🎮 조작법

| 키     | 기능         |
|--------|--------------|
|   ↑   | 게임 시작     |
| ← / → | 패들 이동     |

---

## 📁 폴더 구조

📦 BrickBreaker/                   # 최상위 폴더 (솔루션 루트)  
┣ 📜 BrickBreaker.sln             # Visual Studio 솔루션 파일  
  
┣ 📂 BrickBreaker/                # MFC 프로젝트 폴더  
┃ ┣ 📂 res/                       # 리소스 폴더 (아이콘, 이미지 등)  
┃ ┣ 📂 x64/                       # 빌드 출력 폴더 (x64 플랫폼용)  
  
┃ ┣ 📜 BrickBreaker.h/.cpp       # 애플리케이션 클래스 (CWinApp 기반)  
┃ ┣ 📜 BrickBreaker.rc           # 리소스 정의 파일  
┃ ┣ 📜 BrickBreaker.vcxproj*     # Visual Studio 프로젝트 설정 파일들  
  
┃ ┣ 📜 MainFrm.h/.cpp            # 메인 프레임 (CMainFrame 클래스)  
  
┃ ┣ 📜 ChildView.h/.cpp          # 게임이 그려지는 뷰 클래스 (CView 기반)  
  
┃ ┣ 📜 GameManager.h/.cpp        # 게임 전체 흐름 및 상태 관리  
┃ ┣ 📜 GameObject.h              # 모든 게임 오브젝트들의 기본 클래스  
  
┃ ┣ 📜 Ball.h/.cpp               # 공 객체  
┃ ┣ 📜 Paddle.h/.cpp             # 패들(막대) 객체  
┃ ┣ 📜 Brick.h/.cpp              # 벽돌 객체  
┃ ┣ 📜 Item.h/.cpp # 아이템 객체 (예: 패들 크기 증가, 공 개수 증가 등)  
  
┃ ┣ 📜 framework.h               # MFC 기본 프레임워크 설정  
┃ ┣ 📜 pch.h/.cpp                # 미리 컴파일된 헤더 설정  
┃ ┗ 📜 resource.h                # 리소스 ID 정의 파일  
┃ ┗ 📜 targetver.h               # Windows 타겟 버전 설정  
