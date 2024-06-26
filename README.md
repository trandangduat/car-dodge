﻿# Dodgem (Dodge 'em)

Video chơi thử: [Hướng dẫn chơi game Dodgem - 23020023 Trần Đăng Duật - YouTube](https://youtu.be/1CylLfcD-Lo)

# Giới thiệu

Dodgem là một game thuộc thể loại ***endless runner***. Trong Dodgem, người chơi sẽ đảm nhận vai trò lái xe ô tô của mình và phải vượt qua hàng loạt chướng ngại vật cùng với những quái vật nguy hiểm trên đường đi. 

Điểm độc đáo của trò chơi là sự xuất hiện của ***Cửa hàng kỹ năng***, nơi mà người chơi có thể dùng số tiền thu thập được để mua những *kỹ năng đặc biệt*. Điều này giúp tạo ra một môi trường chiến lược, nơi người chơi có thể thể hiện khả năng sáng tạo của mình để đạt được điểm số cao nhất.

# I. Cài đặt

## 1. Dành cho người chơi

Tải ở đây: [Link](https://github.com/trandangduat/car-dodge/releases/)

![](readme_assets/download_option.png)

Sau khi tải về, người chơi giải nén file zip sau đó mở file có tên là ***dodgem.exe*** để bắt đầu chơi.

## 2. Dành cho lập trình viên

Clone repo này về máy.

<img title="" src="readme_assets/gitclone.png" alt="" width="386">

Sau khi clone xong, mở file ***main/cardDodge.cbp***.

Sau đó, cài đặt SDL2 trên Codeblocks như [hướng dẫn này của Lazyfoo](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/codeblocks/index.php) và [đây.](https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/windows/codeblocks/index.php)

# II. Hướng dẫn chơi

## 1. Màn hình bắt đầu

<img title="" src="readme_assets/starting_screen.png" alt="Starting Screen" data-align="left">

Đây là màn hình xuất hiện khi người chơi mới mở game lên. Ở đây, người chơi sử dụng các phím mũi tên ◄ và ► để chuyển qua các độ khó khác nhau. Ở dưới tên độ khó chính là **Highscore** ứng với từng độ khó đó.

Dodgem có tổng cộng 4 độ khó sắp xếp từ bé tới lớn: **Easy, Medium, Hard, Asian**.

Sau khi chọn được độ khó tương ứng, người chơi ấn vào nút **[► PLAY]** để bắt đầu chơi. 

## 2. Màn hình chơi

<img title="" src="readme_assets\playing_screen_objects.png" data-align="left">

*(1): Số lượng mạng sống*, *(2): Số điểm hiện tại và số tiền hiện tại*, *(3): Số đạn còn lại*, *(4): Các kỹ năng đang được sử dụng*, *(5): Xe của người chơi*, *(6): Các chướng ngại vật*, *(7): Quái vật*

### a. Thành phần trong game

#### Xe người chơi

<img title="" src="main/assets/images/car.png" alt="" width="50" data-align="center">

- Xe của người chơi sẽ được điều khiển bằng **chuột hoặc touchpad**.

- Khi chơi được càng lâu, tốc độ di chuyển của xe sẽ càng tăng.

#### Xe chướng ngại vật

- Các xe ô tô xuất hiện **ngẫu nhiên** trên 4 làn đường đã được phân chia bằng đường kẻ.

- Khi xe của người chơi va phải xe chướng ngại vật, người chơi sẽ **mất đi 1 mạng**, đồng thời chướng ngại vật cũng sẽ biến mất.

- Người chơi có thể sử dụng **đạn đuổi** để phá huỷ xe chướng ngại vật. 

#### Quái vật AI

<img title="" src="readme_assets\playing_screen_boss_ulting.png" alt="boss_ulting" data-align="left">

- Di chuyển với *quỹ đạo khó đoán* trên bầu trời.

- Sau một khoảng thời gian hồi chiêu nhất định, quái vật sẽ định vị và dự đoán vị trí xe người chơi và xuất chiêu liên tục trong vòng khoảng **1 giây**.

- Khi kích hoạt chiêu, quái vật sẽ tiêu diệt **toàn bộ** sinh vật trên đường chiêu thức của nó, bao gồm cả xe chướng ngại vật và xe người chơi.

- Người chơi khi dính phải chiêu của quái vật sẽ bị **mất đi 1 mạng.**

#### Đạn đuổi

- Được kích hoạt bằng cách **click chuột trái**.

- Đường đi mặc định của đạn là đường thằng hàng với xe của người chơi.

- Nếu xác định được xe chướng ngại vật *có khả năng va chạm với xe người chơi*, viên đạn sẽ lập tức thay đổi quỹ đạo bay và phá huỷ chướng ngại trong thời gian chớp mắt.

#### Kỹ năng

Các kỹ năng được chia thành 3 hạng và 2 loại:

| Hạng kỹ năng | Mô tả                                                            |
| ------------ | ---------------------------------------------------------------- |
| Hạng 1       | Những kỹ năng có khả năng thay đổi tình hình ngay lập tức.       |
| Hạng 2       | Những kỹ năng bổ trợ giúp cho việc di chuyển/thu thập.           |
| Hạng 3       | Những kỹ năng được sử dụng kết hợp với các kỹ năng hạng cao hơn. |

| Loại kỹ năng | Mô tả                                                                                                                                                    |
| ------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Bị động      | Đây là những kỹ năng được kích hoạt ngay sau khi mua, sau một khoảng thời gian là **10 giây**, các kỹ năng này sẽ hết hạn sử dụng. (Ví dụ: Coins Magnet) |
| Chủ động     | Đây là những kỹ năng sau khi mua người chơi sẽ được kích hoạt bất kỳ lúc nào muốn. (Ví dụ: x10 Bullets)                                                  |

#### Cửa hàng kỹ năng

<img title="" src="readme_assets\playing_screen_store.png" alt="Store" data-align="left">

- **Mở/Đóng** cửa hàng bằng cách nhấn nút <SPACE> trên bàn phím.

- Cửa hàng có 3 mục tương ứng với từng hạng của các kỹ năng.

- Từng ô kỹ năng sẽ bao gồm: Tên kỹ năng, Tóm tắt tác dụng và Số đồng tiền cần để mua kỹ năng.

- Ấn vào từng ô kỹ năng để có thể mua kỹ năng. Một kỹ năng sẽ chỉ được mua một lần duy nhất trước khi reset lại cửa hàng.

- Sau mỗi khoảng thời gian cố định, cửa hàng sẽ được reset một cách ngẫu nhiên để đảm bảo kích thích sáng tạo của người chơi.

### b. Độ khó

| Độ khó | Mô tả                                                                                                                          |
| ------ | ------------------------------------------------------------------------------------------------------------------------------ |
| Easy   | Không tồn tại quái vật, 20 viên đạn đuổi khi xuất phát, 5 mạng.                                                                |
| Medium | 10 viên đạn đuổi khi xuất phát, thời gian hồi chiêu của quái vật là 10 giây, 5 mạng.                                           |
| Hard   | 5 viên đạn đuổi khi xuất phát, thời gian hồi chiêu của quái vật là 5 giây, 3 mạng, tốc độ khi xuất phát gấp 2 lần bình thường. |
| Asian  | 0 viên đạn đuổi khi xuất phát, thời gian hồi chiêu của quái vật là 3 giây, 2 mạng, tốc độ khi xuất phát gấp 4 lần bình thường. |

## 3. Màn hình kết thúc

<img title="" src="readme_assets\gameover_screen.png" alt="Game over" data-align="left">

Khi người chơi không còn mạng nào, màn hình kết thúc sẽ hiện ra và cho người chơi biết được điểm số ván chơi đó, cũng như điểm cao nhất của độ khó hiện tại. Nếu người chơi đạt được điểm số cao nhất trong ván chơi hiện tại thì một con tem <img title="" src="main/assets/images/game_over/highscore_stamp.png" alt="" width="50"> sẽ hiện ra để thông báo với người chơi.

Để chơi ván mới, người chơi ấn vào nút **[HOME]** để trở về **màn hình bắt đầu**.

# III. Source code

Dưới đây là giải thích một số hàm, file quan trọng trong source code trò chơi:

## `main.cpp`

- `void resetGame()` reset lại các thông số của game để chuẩn bị vào ván chơi mới.

- `bool handleEvent (SDL_Event e)` xử lý các event do người chơi thực hiện (Ví dụ: chuột phải, nút <Space>, ...)

- `void closeGame()` free/release toàn bộ những tài nguyên được sử dụng trong khi chạy trò chơi và đóng các subsystems của SDL.

- `void generateColumnRanges()` phân chia đường lớn thành 4 làn đường nhỏ dựa theo kích cỡ của lề đường và độ rộng mỗi làn đường theo đường phân cách.

- `void updateBgVelocity()` tăng tốc độ chạy sau mỗi khoảng thời gian nhất định, đảm độ khó tăng dần theo thời gian.

- `SDL_Point getNearestObstacle (float x, float y)` sử dùng thuật toán tìm kiếm heuristic để trả về chướng ngại vật có khả năng gây nguy hiểm cho xe người chơi. Hàm này được dùng để thay đổi toạ độ của *đạn đuổi*.

- `void render()` render toàn bộ những thành phần trong trò chơi lên màn hình.

- `void update()` update toàn bộ thông số của các thành phần trong trò chơi.

## `gamewindow.cpp`

- `bool GameWindow::init()` khởi tạo những subsystems của SDL.

- `SDL_Texture* GameWindow::loadTexture (std::string path)` chuyển tài nguyên theo đường dẫn thành dạng biến con trỏ SDL_Texture.

- `void GameWindow::blit(...)` in texture lên màn hình.

## `gamestate.cpp`

- Đây là file chứa toàn bộ thông số của một ván game mới, bao gồm: `score` (số điểm), `lives` (số mạng sống), `coins` (số tiền hiện tại), `bullets` (số đạn còn lại), ...

- `std::vector<long long> getHighScoreFromDataFile (std::string path)` lấy thông tin điểm cao nhất từ file thông tin chuyển sang dạng mảng động `std::vector`.

- `void updateHighScoreToDataFile (std::string path, const std::vector<long long> &highScores)` update lại thông tin điểm cao nhất sang file để lưu trữ.

## `assets.cpp`

- `void loadMedia (GameWindow* win)` load toàn bộ các tài nguyên vào trong chương trình dưới dạng biến con trỏ.

- `void freeMedia()` free/release toàn bộ tài nguyên khi không còn sử dụng nữa.

## `params.hpp`

- Đây là file khai báo những thông số cơ bản của trò chơi như `SCREEN_WIDTH` (chiều rộng màn hình), `OBSTACLE_WIDTH` (chiều rộng xe chướng ngại vật), `BOSS_COOLDOWN` (thời gian hồi chiêu của quái vật), ...

## `vfx.hpp`

- Đây là file chứa `struct VFX` để sử dụng vào những thành phần hiệu ứng hình ảnh trong game (logo, chiêu của quái vật, hiệu ứng tăng tốc, ...).

## `hud.cpp`

- `void HUD::renderGameOverScreen()` in ra màn hình kết thúc game.

- `void HUD::renderStore(...)` in ra màn hình **Cửa hàng kỹ năng**

- File này còn dùng để in ra những thông số như Số điểm, số tiền, số đạn còn lại lên trên màn hình chơi.

## `boss.cpp`

- `void Boss::move (int x, int y)` update vị trí và trạng thái của quái vật theo thời gian, nếu thời gian hồi chiêu đã hết thì quái vật sẽ kiểm tra toạ độ của xe người chơi theo tham số vào là (x, y) để ra chiêu:

- ```cpp
          if (this->mState == BOSS_MOVING || (this->mState == BOSS_DELAY && this->mTimer->elapsedTime() < 1000)) {
              if (this->mState == BOSS_DELAY) {
                  this->destination = {x, y};
              }
              else {
                  this->destination = {rand() % SCREEN_WIDTH, rand() % 200};
              }
          }
  ```

## `bullet.cpp`

- `void Bullet::move (SDL_Point target, float dTime)` update vị trí của đạn đuổi theo toạ độ điểm `target`.

## `abilities.cpp`

- `void loadAbilitiesFromFiles()` load thông tin những Kỹ năng từ file data và chuyển thành những biến sử dụng được trong chương trình.

- `void activeAbility (GameState* gState, int tier, int id)` kích hoạt sử dụng những kỹ năng đã được mua.

- `void renderActiveAbilities (GameWindow* gWin, HUD* gHud)` in ra những kỹ năng vẫn còn hạn sử dụng lên màn hình (góc trái dưới).

- `void updateActiveAbilities()` kiểm tra hạn sử dụng của những kỹ năng đã được mua.

## `logic.cpp`

- `bool checkCollision (SDL_Rect A, SDL_Rect B)` kiểm tra sự giao nhau giữa hai hình chữ nhật, được sử dụng để kiểm tra va chạm với chướng ngại vật và chiêu của quái vật lên xe người chơi.

# IV. Credits

- Font **KarenFat** của [PaulSpades](https://paul-spades.itch.io/)

- Font **Avenue Pixel** của [J. D. Jimenez](https://jdjimenez.itch.io/avenue-pixel)

- **Font pack** của [zingot](https://zingot.itch.io/fontpack)

- **Sprite xe ô tô** của [tokka](https://tokka.itch.io/top-down-car)

- **Sprite quái vật** của [elthen](https://elthen.itch.io/2d-pixel-art-cacodaemon-sprites)

- **Sprite đồng tiền** của [TotusLotus](https://totuslotus.itch.io/pixel-coins)

- **Background** của [Mariusz Szulc](https://www.behance.net/gallery/29946977/Must-Deliver-Backgrounds)

- Nhạc nền của **Zombie Tsunami**

- Phần mềm đồ hoạ được sử dụng: [LibreSprite](https://libresprite.github.io/#!/) và [Photopea](https://www.photopea.com/).
