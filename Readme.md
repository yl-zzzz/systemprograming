<h3> 2021-2semester Systemprograming Project</h3>

<h1 align="center">Inventory clearance program using sockets  </h1>
<h3>Team Member</h3>
<h4>유예린 : 기획,프로그래밍,보고서 작성, 발표 </h4>

------------------------------------------

# 1. 개발 계기
> &nbsp;동네에 크지 않은 슈퍼나, 편의점 등 재고 조사를 하는 곳에서 종이에 수기로 재고 정리를 하는 곳을 빈번히 본 적이 있다.
> 종이에 수기로 작성할 경우 종이가 사라지면 다시 종이를 프린트해서 해야하는 번거로움이 있다.
> 그리고 작은 매장 같은 경우에는 구매자들이 슈퍼에 갔을 때 사고자 하는 제품이 없어서 헛걸음을 하게 되는 경우가 있었다. 
> 저도 종종 집 근처 작은 마트에 가서 원하는 물건이 없거나 다 팔렸을 때 조금은 짜증이 난 적이 있었고, 미리 재고가 있는지 알았다면 
> 좋았을거라는 생각을 하곤 했다. 그래서 남아있는 재고들을 확인하고 온라인으로 미리 주문을 해두고 매장에 간다면 좋을 것 같고, 
> 작은 매장인 경우 재고가 많지 않기때문에 온라인으로 주문된 재고를 바로바로 체크하기 좋을 것 같아서 이러한 프로젝트를 기획하게 되었다. 

<br>

# 2. 프로젝트 개발 내용
  ### 1. 인터넷 소켓을 사용하여 클라이언트와 연결하여 정보를 주고 받는다.</br>  
  ### 2. fork를 사용해서 다중 프로세스를 생성 할 수 있게 한다.</br>
  ### 3. 메모리매핑을 사용해서 재고 리스트가 있는 파일에 접근해 해당 파일을 읽고 쓰기가 가능하게 한다.</br>
  ### 4. 서버에서 연결된 파일을 수정한다.</br>
  **사용언어 : C**
  </br>
  <img src="/Readme_img/socket.PNG" width="300" height = "350"></p></br>
# 3. 프로젝트 개발 결과물 (다이어그램)
<p align="center">
<img src="/Readme_img/diagram.PNG" width=450 height = 350></p>

  ### sever.c) 서버에서는 소켓을 사용하여 클라이언트와 연결을 하고, fork()를 이용해 다중 프로세스를 만들 수 있도록 하였다.
  ### 서버에서는 클라이언트로 부터 원하는 메뉴얼을 입력받아서, mmap을 사용해서 연 파일에 재고를 추가하거나 제거하는 역할을 하게된다.
  ### Offline_client.c) 오프라인 클라이언트 서버에서는 재고를 추가하고, 삭제하는 작업을 수행하는 클라이언트이다.
  ### 오프라인 클라이언트가 원하는 매뉴얼을 send를 통해 서버에게 전달해주면 서버가 작업을 수행하고 재고 목록을 확인 할 수 있다.
  ### Online_client.c) 온라인 클라이언트는 물건을 사고, 자신이 산 물건 목록을 확인할 수 있는 클라이언트이다.
  </br>
  
# 4. 프로젝트 실행 방법 및 결과
  ### 1. 터미널에서 서버 클라이언트를 컴파일 하고, 실행시킨다.
  ### 2. 오프라인에서 계산을 하고, 재고를 추가하려면 offline_client를 컴파일하고 실행시킨다.
  ####   2-2. 온라인에서도 물건을 사려면 online_client를 컴파일하고 실행시킨다.
  ### 3. 클라이언트가 서버와 연결이 되었다면 클라이언트는 메뉴얼중에서 하나를 선택한다.
  ### 4. Add_inventory를 선택하였다면 추가하려는 제품의 이름과 수량을 적어준다.
  ### 4-2. 다시 메뉴얼로 나가려면 exit를 입력한다.
  ### 5. Calc를 선택하였다면 계산하려는 제품의 이름과 수량을 적어준다.
  ### 6. 종료시키려면 메뉴얼창에서 Exit를 선택하면 된다.
  ### 7. 재고 목록을 보려면 View inventory list를 선택하면 목록이 출력된다.
  ### 8. 온라인에서도 동일한 방법으로 원하는 메뉴얼을 선택하고 실행하면 된다.
# 실행 결과 캡쳐
<img src="/Readme_img/demoCapture1.PNG" width=500 height = 450></p>
<img src="/Readme_img/demoCapture2.PNG" width=500 height = 450></p>
<img src="/Readme_img/demoCapture3.PNG" width=500 height = 450></p>
<img src="/Readme_img/demoCapture4.PNG" width=500 height = 450></p>
 
 
