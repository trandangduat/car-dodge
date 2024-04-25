# Dodgem (Dodge 'em)

Video chơi thử:

# Giới thiệu

Dodgem là một game thuộc thể loại ***endless runner***. Trong Dodgem, người chơi sẽ đảm nhận vai trò lái xe ô tô của mình và phải vượt qua hàng loạt chướng ngại vật cùng với những quái vật nguy hiểm trên đường đi. 

Điểm độc đáo của trò chơi là sự xuất hiện của ***Cửa hàng kỹ năng***, nơi mà người chơi có thể dùng số tiền thu thập được để mua những *kỹ năng đặc biệt*. Điều này giúp tạo ra một môi trường chiến lược, nơi người chơi có thể thể hiện khả năng sáng tạo của mình để đạt được điểm số cao nhất.

# I. Cài đặt

# II. Hướng dẫn chơi

## 1. Màn hình bắt đầu

<img title="" src="file:///D:/CPPGames/car-dodge/readme_assets/starting_screen.png" alt="Starting Screen" data-align="left">

Đây là màn hình xuất hiện khi người chơi mới mở game lên. Ở đây, người chơi sử dụng các phím mũi tên ◄ và ► để chuyển qua các độ khó khác nhau. Ở dưới tên độ khó chính là **Highscore** ứng với từng độ khó đó.

Dodgem có tổng cộng 4 độ khó sắp xếp từ bé tới lớn: **Easy, Medium, Hard, Asian**.

Sau khi chọn được độ khó tương ứng, người chơi ấn vào nút **[► PLAY]** để bắt đầu chơi. 

## 2. Màn hình chơi

![](D:\CPPGames\car-dodge\readme_assets\playing_screen_objects.png)

*(1): Số lượng mạng sống*, *(2): Số điểm hiện tại và số tiền hiện tại*, *(3): Số đạn còn lại*, *(4): Các kỹ năng đang được sử dụng*, *(5): Xe của người chơi*, *(6): Các chướng ngại vật*, *(7): Quái vật*

### Thành phần trong game

#### Xe người chơi: <img title="" src="file:///D:/CPPGames/car-dodge/main/assets/images/car.png" alt="" width="50" data-align="center">

- Xe của người chơi sẽ được điều khiển bằng **chuột hoặc touchpad**.

- Khi chơi được càng lâu, tốc độ di chuyển của xe sẽ càng tăng.

#### Xe chướng ngại vật:

<img title="" src="file:///D:/CPPGames/car-dodge/main/assets/images/cars.png" alt="" data-align="center" width="177">

- Các xe ô tô xuất hiện **ngẫu nhiên** trên 4 làn đường đã được phân chia bằng đường kẻ.

- Khi xe của người chơi va phải xe chướng ngại vật, người chơi sẽ **mất đi 1 mạng**, đồng thời chướng ngại vật cũng sẽ biến mất.

- Người chơi có thể sử dụng **đạn đuổi** để phá huỷ xe chướng ngại vật. 

#### Quái vật:

<img title="" src="file:///D:/CPPGames/car-dodge/main/assets/images/boss_near_ulting_sprite.png" alt="" data-align="center" width="220">

![boss_ulting](D:\CPPGames\car-dodge\readme_assets\playing_screen_boss_ulting.png)

- Di chuyển với *quỹ đạo khó đoán* trên bầu trời.

- Sau một khoảng thời gian hồi chiêu nhất định, quái vật sẽ định vị và dự đoán vị trí xe người chơi và xuất chiêu liên tục trong vòng khoảng **1 giây**.

- Người chơi khi dính phải chiêu của quái vật sẽ bị **mất đi 1 mạng.**

#### Đạn đuổi

<img title="" src="file:///D:/CPPGames/car-dodge/main/assets/images/items/bullet.png" alt="" data-align="center" width="50">

- Được kích hoạt bằng cách click chuột trái.

- Đường đi mặc định của đạn là đường thằng hàng với xe của người chơi.

- Nếu xác định được xe chướng ngại vật *có khả năng va chạm với xe người chơi*, viên đạn sẽ lập tức thay đổi quỹ đạo bay và phá huỷ chướng ngại trong thời gian chớp mắt.

#### Kỹ năng

Kỹ năng được chia thành 3 hạng:

- **Hạng 1**: Những kỹ năng có khả năng thay đổi tình hình lập tức.

- **Hạng 2**: Những kỹ năng bổ trợ giúp cho việc di chuyển/thu thập.

- **Hạng 3**: Những kỹ năng vô bổ hoặc có thể được sử dụng kết hợp với các kỹ năng hạng trên.

Có hai loại kỹ năng:

- **Bị động**: Đây là những kỹ năng được kích hoạt ngay sau khi mua, sau một khoảng thời gian là **10 giây**, các kỹ năng này sẽ hết hạn sử dụng. (Ví dụ: Coins Magnet)

- **Chủ động**: Đây là những kỹ năng sau khi mua người chơi sẽ được kích hoạt bất kỳ lúc nào muốn. (Ví dụ: x10 Bullets)

#### Cửa hàng kỹ năng

![store](D:\CPPGames\car-dodge\readme_assets\playing_screen_store.png)

- **Mở/Đóng** cửa hàng bằng cách nhấn nút <SPACE> trên bàn phím.

- Cửa hàng có 3 mục tương ứng với từng hạng của các kỹ năng.

- Từng ô kỹ năng sẽ bao gồm: Tên kỹ năng, Tóm tắt tác dụng và Số đồng tiền cần để mua kỹ năng.

- Ấn vào từng ô kỹ năng để có thể mua kỹ năng. Một kỹ năng sẽ chỉ được mua một lần duy nhất trước khi reset lại cửa hàng.

- Sau mỗi khoảng thời gian cố định, cửa hàng sẽ được reset một cách ngẫu nhiên để đảm bảo kích thích sáng tạo của người chơi.

### Độ khó

- **Easy**: không tồn tại quái vật, 20 viên đạn đuổi khi xuất phát, 5 mạng.

- **Medium**: 10 viên đạn đuổi khi xuất phát, thời gian hồi chiêu của quái vật là 10 giây, 5 mạng.

- **Hard**: 5 viên đạn đuổi khi xuất phát, thời gian hồi chiêu của quái vật là 5 giây, 3 mạng, tốc độ khi xuất phát gấp 2 lần bình thường.

- **Asian**: 0 viên đạn đuổi khi xuất phát, thời gian hồi chiêu của quái vật là 3 giây, 2 mạng, tốc độ khi xuất phát gấp 4 lần bình thường.

## 3. Màn hình kết thúc

![game_over](D:\CPPGames\car-dodge\readme_assets\gameover_screen.png)

Khi người chơi không còn mạng nào, màn hình kết thúc sẽ hiện ra và cho người chơi biết được điểm số ván chơi đó, cũng như điểm cao nhất của độ khó hiện tại. Nếu người chơi đạt được điểm số cao nhất trong ván chơi hiện tại thì một con tem <img title="" src="file:///D:/CPPGames/car-dodge/main/assets/images/game_over/highscore_stamp.png" alt="" width="50"> sẽ hiện ra để thông báo với người chơi.

Để chơi ván mới, người chơi ấn vào nút **[HOME]** để trở về **màn hình bắt đầu**.
