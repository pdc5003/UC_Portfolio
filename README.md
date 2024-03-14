# 포트폴리오 기술서

![포트폴리오썸네일1](https://github.com/pdc5003/UC_Portfolio/assets/34324684/ccc0098c-1507-4c39-8eab-845f2b260aab)


- **프로젝트 개요**
    - 프로젝트 설명 : Unreal Engin4를 활용한 액션게임 제작
    - 개발기간 : 2021.04.12 ~ 2021.07.02(82일)
    - 개발환경 : Unreal Engine4 (4.26), Unreal Engine C++
    

---

- **프로젝트 사용 기술**
    - **Execution System**
        
        적용 대상 : 플레이어
        
        클래스 : UPFinisherComponent
        
        사용 이유 : 갓 오브 워의 처형 모션처럼 사용자로 하여금 카타르시스를 느끼게 해주고 레벨 시퀀스를 사용해 보다 역동적인 움직임을 보여주는데 집중했습니다.
        
        [https://youtu.be/Y6ALM4C9PS4](https://youtu.be/Y6ALM4C9PS4)
        
        특이사항 : 처형 레벨 시퀀스가 재생될 때 카메라의 위치와 각도를 맞추기 위해 시네마틱 카메라를 관리하는 액터를 미리 생성하고 처형 모션을 실행 할 시 바인딩과 언바인딩을 통해 처리했습니다.
        
        ---
        
    - **Axe Throw&Recall**
        
        적용 대상 : 플레이어
        
        클래스 : APArmed_Axe
        
        사용 이유 : 갓 오브 워에서 리바이어던 도끼를 던지고 회수하는 시스템이 게임을 해본 유저들에게 많은 호평을 받았고 제가 만든 포트폴리오의 모티브도 갓 오브 워를 기반으로 했기 때문에 시스템을 구현해보고자 하였습니다.
        
        [https://youtu.be/YLxRH9EsnDI](https://youtu.be/YLxRH9EsnDI)
 
      ![도끼회수헤더](https://github.com/pdc5003/UC_Portfolio/assets/34324684/9e5cc8f0-a749-44cc-a02e-d04be789e3f5)
      ![도끼회수CPP](https://github.com/pdc5003/UC_Portfolio/assets/34324684/0b21f843-96ea-45af-8166-1d600abe0697)
        
  
        특이사항 : 도끼를 던지고 나서 회수하는 과정을 베지에 곡선을 이용해서 구현했습니다.
        
        ---
        
    - **AI 패턴**
        
        **Boss AI Pattern Table**
        
        적용대상 : Boss AI
        
        클래스 : ACAIController_Boss
        
        사용 이유 : 보스AI의 일반적인 AI와 다르게 플레이어에게 좀 더 압박감을 줄 수 있는 다채로운 연계 공격이나 견제 공격 방식이 필요했으며, 액션에 따른 상태들을 데이터 테이블로 관리하여 수정을 편하게 할 수 있게 하였습니다.
 
        ![AI보스패턴테이블](https://github.com/pdc5003/UC_Portfolio/assets/34324684/28d39d8b-51c0-4108-b83b-b7d5a6ae030f)
        ![보스패턴코드일부](https://github.com/pdc5003/UC_Portfolio/assets/34324684/c818c3c4-166a-4f31-81d4-d1dd8a3c6607)
     
        
        데이터 행의 갯수 만큼 패턴 리스트를 등록
        
        특이사항 : 데이터 테이블에 있는 행 갯수 만큼 Map의 크기를 할당하고 key값을 
        아이디, value값을 델리게이트를 배열로 설정해서 플레이어와 전투를 할 때 임의의 아이디 값을 뽑고 그에 해당되는 패턴을 순차적으로 실행되는 방식으로 구현했습니다.
        
        ---
        
    - **Command System**
        
        적용 대상 : 플레이어
        
        클래스 : UPCommandBuffer
        
        사용 이유 : 커맨드 조합을 통해 보다 능동적이고 역동적인 액션을 유도하기 위해 사용했습니다.
        
        ![LockOnDash](https://github.com/pdc5003/UC_Portfolio/assets/34324684/504eb977-220a-4584-b5c4-4361c1fd0552)
        
        대쉬 공격
        
        ![LockOnUpper](https://github.com/pdc5003/UC_Portfolio/assets/34324684/5ee6bc61-02b3-4c6c-980a-002f9950ae58)
        
        어퍼 공격
        
        ![LockOnRound](https://github.com/pdc5003/UC_Portfolio/assets/34324684/0dff3a42-ae1b-4fb9-97cf-1871141812a1)
        
        주변 광역 공격
        
        특이사항 : 커맨드 리스트와 현재 사용자가 입력한 커맨드가 동일한 지를 비교해야되며 
        커맨드가 들어온 순서를 보장해야되기 때문에 Queue를 이용해 입력한 순서를 보장했고, Map의 키 값에 커맨드 리스트 등록해 탐색시간을 줄이는데 초점을 맞췄습니다.
        
        ---
        
    - **Action System**
        
        적용 대상 : 플레이어,AI
        
        클래스 : APCombatAction 및 파생 클래스
        
        ![무기 액션](https://github.com/pdc5003/UC_Portfolio/assets/34324684/365ab9bf-b911-4b14-8602-0ca732dd72b6)
        
        사용 이유 : 무기 및 무장별 애니메이션과 파티클이 다르게 작동되야 되기 때문에 최상위 부모클래스인 APCombatAction에서 해당 메서드들을 정의하여 오버라이드 하는 방식으로 처리했습니다.
        
        ---
        
    - **Inverse Kinematic**
        
        적용 대상 : 플레이어 , AI
        
        클래스 : UCFeetComponent
        
        사용 이유 : 경사면이나 계단을 오를 때 발을 딛는 모습에 어색함을 줄이기 위해 사용했습니다.
        
        ![IK](https://github.com/pdc5003/UC_Portfolio/assets/34324684/a65ef2e9-5649-4a6e-8a18-7530088a963a)
        
        FeetIK
        

---

- **작업 내역**
    - Boss AI 패턴 추가 후 테스트
        
        [https://youtu.be/eizSq9uBRdw](https://youtu.be/eizSq9uBRdw)
        
    
    - Boss AI 기본 패턴 테스트
        
        [https://youtu.be/cPv1QHDEkWk](https://youtu.be/cPv1QHDEkWk)
        
    
    - 플레이어 퍼펙트 가드
        
        [https://youtu.be/bDEAimwMZ3k](https://youtu.be/bDEAimwMZ3k)
        
    
    - 플레이어 가드시 밀림
        
        [https://youtu.be/GsaAVsJVlMg](https://youtu.be/GsaAVsJVlMg)
        
    
    - 플레이어 카운터 어택 및 도끼 던지기로 적 홀딩
        
        [https://youtu.be/1fsnBOEdLlA](https://youtu.be/1fsnBOEdLlA)
        
    
    - 신규 AI 타입 추가
        
        [https://youtu.be/Nfgy9NEIvLM](https://youtu.be/Nfgy9NEIvLM)
        
    
    - AI와 전투 프로토타입
        
        [https://youtu.be/6OyoE2FRH8Y](https://youtu.be/6OyoE2FRH8Y)
        
    
    - AI 첫 번째 타입 추가
        
        [https://youtu.be/DHYD_wUxLsM](https://youtu.be/DHYD_wUxLsM)
        
    
    - 타겟팅 시스템
        
        [https://youtu.be/2mbJ-f5UTTg](https://youtu.be/2mbJ-f5UTTg)
        
    
    - 무기 트레일 및 이펙트 추가
        
        [https://youtu.be/mtZfdGCWQLs](https://youtu.be/mtZfdGCWQLs)
        
    
    - 도끼 무기 스킬 추가
        
        [https://youtu.be/XOvUzOq-1JU](https://youtu.be/XOvUzOq-1JU)
        
    
    - 공중 콤보 최종버전
        
        [https://youtu.be/BWwf8iDHnGw](https://youtu.be/BWwf8iDHnGw)
        
    
    - 공중 콤보 프로토타입 제작
        
        [https://youtu.be/jfx2F0dza5c](https://youtu.be/jfx2F0dza5c)
        
    
    - 처형 모션 베타버전2
        
        [https://youtu.be/2inw71eSs6c](https://youtu.be/2inw71eSs6c)
        
    
    - 처형 모션 베타버전1
        
        [https://youtu.be/0PDbg8oedFo](https://youtu.be/0PDbg8oedFo)
        
    
    - 처형 모션 프로토타입 2
        
        [https://youtu.be/GS9btPhI6bs](https://youtu.be/GS9btPhI6bs)
        
    
    - 처형모션 프로토타입 1
        
        [https://youtu.be/7iMmHpsXDQ0](https://youtu.be/7iMmHpsXDQ0)
        
    
    - 공격 커맨드 기술
        
        [https://youtu.be/YOq8nUCX2y4](https://youtu.be/YOq8nUCX2y4)
        
    
    - 도끼 던진 후 맨손공격 상태로 전환
        
        [https://youtu.be/ThDWeFjBdq0](https://youtu.be/ThDWeFjBdq0)
        
    
    - 도끼 던지기 마지막 수정안
        
        [https://youtu.be/3PUuL2k45aE](https://youtu.be/3PUuL2k45aE)
        
    
    - 도끼 던지기 프로토타입
        
        [https://youtu.be/mR1wGQHnf2E](https://youtu.be/mR1wGQHnf2E)
        
    
    - 기본 이동 및 공격
        
        [https://youtu.be/RZUbGu_849g](https://youtu.be/RZUbGu_849g)
        
    
    ---
