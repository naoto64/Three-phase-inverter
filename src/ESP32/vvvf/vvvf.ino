#define U_PIN_IN 33
#define U_PIN_SD 25
#define V_PIN_IN 26
#define V_PIN_SD 27
#define W_PIN_IN 14
#define W_PIN_SD 12

#define TABLE_LEN 256
#define CTRL_RATE 20000
#define FREQ_LEN_ACCEL 9
#define FREQ_LEN_DECEL 1

hw_timer_t * timer = NULL;

// 非同期モードの周波数配列（加速）
const double freq_table_accel[FREQ_LEN_ACCEL][3] = {{175, 0.5, 0}, {196, 1.0, 0}, {223, 1.5, 0}, {233, 2.0, 0}, {262, 2.5, 0}, {294, 3.0, 0}, {311, 3.5, 0}, {350, 4.0, 0}, {400, 4.5, 0}};
// 非同期モードの周波数配列（減速）
const double freq_table_decel[FREQ_LEN_DECEL][3] = {{400, 24, 0}};
// 変調波（正弦波・交流波形）
const double sin_table[TABLE_LEN] = {0, 6.258, 12.512, 18.759, 24.994, 31.215, 37.416, 43.595, 49.748, 55.871, 61.96, 68.012, 74.023, 79.989, 85.907, 91.773, 97.584, 103.337, 109.027, 114.651, 120.206, 125.689, 131.096, 136.424, 141.67, 146.831, 151.903, 156.884, 161.77, 166.559, 171.248, 175.833, 180.312, 184.683, 188.943, 193.088, 197.118, 201.028, 204.818, 208.484, 212.025, 215.438, 218.721, 221.872, 224.89, 227.772, 230.517, 233.123, 235.589, 237.913, 240.094, 242.13, 244.02, 245.763, 247.358, 248.804, 250.1, 251.246, 252.24, 253.082, 253.772, 254.309, 254.693, 254.923, 255, 254.923, 254.693, 254.309, 253.772, 253.082, 252.24, 251.246, 250.1, 248.804, 247.358, 245.763, 244.02, 242.13, 240.094, 237.913, 235.589, 233.123, 230.517, 227.772, 224.89, 221.872, 218.721, 215.438, 212.025, 208.484, 204.818, 201.028, 197.118, 193.088, 188.943, 184.683, 180.312, 175.833, 171.248, 166.559, 161.77, 156.884, 151.903, 146.831, 141.67, 136.424, 131.096, 125.689, 120.206, 114.651, 109.027, 103.337, 97.584, 91.773, 85.907, 79.989, 74.023, 68.012, 61.96, 55.871, 49.748, 43.595, 37.416, 31.215, 24.994, 18.759, 12.512, 6.258, 0, -6.258, -12.512, -18.759, -24.994, -31.215, -37.416, -43.595, -49.748, -55.871, -61.96, -68.012, -74.023, -79.989, -85.907, -91.773, -97.584, -103.337, -109.027, -114.651, -120.206, -125.689, -131.096, -136.424, -141.67, -146.831, -151.903, -156.884, -161.77, -166.559, -171.248, -175.833, -180.312, -184.683, -188.943, -193.088, -197.118, -201.028, -204.818, -208.484, -212.025, -215.438, -218.721, -221.872, -224.89, -227.772, -230.517, -233.123, -235.589, -237.913, -240.094, -242.13, -244.02, -245.763, -247.358, -248.804, -250.1, -251.246, -252.24, -253.082, -253.772, -254.309, -254.693, -254.923, -255, -254.923, -254.693, -254.309, -253.772, -253.082, -252.24, -251.246, -250.1, -248.804, -247.358, -245.763, -244.02, -242.13, -240.094, -237.913, -235.589, -233.123, -230.517, -227.772, -224.89, -221.872, -218.721, -215.438, -212.025, -208.484, -204.818, -201.028, -197.118, -193.088, -188.943, -184.683, -180.312, -175.833, -171.248, -166.559, -161.77, -156.884, -151.903, -146.831, -141.67, -136.424, -131.096, -125.689, -120.206, -114.651, -109.027, -103.337, -97.584, -91.773, -85.907, -79.989, -74.023, -68.012, -61.96, -55.871, -49.748, -43.595, -37.416, -31.215, -24.994, -18.759, -12.512, -6.258};
// 搬送波（三角波・制御波形）
const double tri_table[TABLE_LEN] = {0, -6.259, -12.517, -18.776, -25.035, -31.293, -37.552, -43.81, -50.069, -56.328, -62.586, -68.845, -75.104, -81.362, -87.621, -93.88, -100.138, -106.397, -112.656, -118.914, -125.173, -131.431, -137.69, -143.949, -150.207, -156.466, -162.725, -168.983, -175.242, -181.501, -187.759, -194.018, -200.277, -206.535, -212.794, -219.052, -225.311, -231.57, -237.828, -244.087, -250.346, -256.604, -262.863, -269.122, -275.38, -281.639, -287.898, -294.156, -300.415, -306.673, -312.932, -319.191, -325.449, -331.708, -337.967, -344.225, -350.484, -356.743, -363.001, -369.26, -375.518, -381.777, -388.036, -394.294, -400.553, -394.294, -388.036, -381.777, -375.518, -369.26, -363.001, -356.743, -350.484, -344.225, -337.967, -331.708, -325.449, -319.191, -312.932, -306.673, -300.415, -294.156, -287.898, -281.639, -275.38, -269.122, -262.863, -256.604, -250.346, -244.087, -237.828, -231.57, -225.311, -219.052, -212.794, -206.535, -200.277, -194.018, -187.759, -181.501, -175.242, -168.983, -162.725, -156.466, -150.207, -143.949, -137.69, -131.431, -125.173, -118.914, -112.656, -106.397, -100.138, -93.88, -87.621, -81.362, -75.104, -68.845, -62.586, -56.328, -50.069, -43.81, -37.552, -31.293, -25.035, -18.776, -12.517, -6.259, 0, 6.259, 12.517, 18.776, 25.035, 31.293, 37.552, 43.81, 50.069, 56.328, 62.586, 68.845, 75.104, 81.362, 87.621, 93.88, 100.138, 106.397, 112.656, 118.914, 125.173, 131.431, 137.69, 143.949, 150.207, 156.466, 162.725, 168.983, 175.242, 181.501, 187.759, 194.018, 200.277, 206.535, 212.794, 219.052, 225.311, 231.57, 237.828, 244.087, 250.346, 256.604, 262.863, 269.122, 275.38, 281.639, 287.898, 294.156, 300.415, 306.673, 312.932, 319.191, 325.449, 331.708, 337.967, 344.225, 350.484, 356.743, 363.001, 369.26, 375.518, 381.777, 388.036, 394.294, 400.553, 394.294, 388.036, 381.777, 375.518, 369.26, 363.001, 356.743, 350.484, 344.225, 337.967, 331.708, 325.449, 319.191, 312.932, 306.673, 300.415, 294.156, 287.898, 281.639, 275.38, 269.122, 262.863, 256.604, 250.346, 244.087, 237.828, 231.57, 225.311, 219.052, 212.794, 206.535, 200.277, 194.018, 187.759, 181.501, 175.242, 168.983, 162.725, 156.466, 150.207, 143.949, 137.69, 131.431, 125.173, 118.914, 112.656, 106.397, 100.138, 93.88, 87.621, 81.362, 75.104, 68.845, 62.586, 56.328, 50.069, 43.81, 37.552, 31.293, 25.035, 18.776, 12.517, 6.259};
double sin_index = 0; // 正弦波の位相（0-255）
double tri_index = 0; // 三角波の位相（0-255）
double freq = 0;      // 変調波の周波数
double ctrl_freq = 0; // 非同期モードの周波数
double voltage = 0;   // 出力電圧
double comp_const = 1;  // 比較係数（非同期モードと同期モード）
unsigned int ctrl_freq_index = 0; // 非同期モードの周波数のインデックス
unsigned char sync_mode = 0;  // 0: 非同期モード, 1: 同期モード
unsigned char pulse_mode = 0; // パルスモード

char U_IN_FLAG = LOW; // U相の出力
char V_IN_FLAG = LOW; // V相の出力
char W_IN_FLAG = LOW; // W相の出力
int accel = 0;  // 加速度

void IRAM_ATTR onTimer() {
  // 出力波形の周波数を計算
  freq = min(max(freq + (double)accel * 0.5 / (double)CTRL_RATE, 0.0), 120.0);
  // 電圧を計算
  voltage = min(0.06 + freq / 55.0, 1.0);
  // 正弦波の位相を計算
  sin_index += (double)TABLE_LEN * freq / (double)CTRL_RATE;

  // 位相が1周したら1周分を引く
  while(sin_index >= TABLE_LEN){
    sin_index -= (double)TABLE_LEN;
  }
  
  // 非同期モード（加速）
  if(freq < freq_table_decel[0][1] && accel > 0){
    sync_mode = 0;  // 非同期モード
    comp_const = 1.0; // 比較係数1
    // インデックスが末尾出ない場合の処理
    if(ctrl_freq_index < FREQ_LEN_ACCEL - 1){
      // 非同期モードの周波数を計算
      if(freq > freq_table_accel[ctrl_freq_index + 1][1]){
        ctrl_freq_index++;
      }
      ctrl_freq = freq_table_accel[((unsigned int)ctrl_freq_index) % FREQ_LEN_ACCEL][0];
      // リニアに変化する場合の処理
      if(freq_table_accel[((unsigned int)ctrl_freq_index) % FREQ_LEN_ACCEL][2] == 1){
        ctrl_freq += (freq_table_accel[ctrl_freq_index + 1][0] - ctrl_freq) * (freq - freq_table_accel[ctrl_freq_index][1]) / freq_table_accel[ctrl_freq_index + 1][1];
      }
    }
    // 非同期モードの位相を計算
    tri_index += (double)TABLE_LEN * ctrl_freq / (double)CTRL_RATE;
  }
  // 非同期モード（減速）
  else if(freq < freq_table_decel[0][1]){
    sync_mode = 0;  // 非同期モード
    comp_const = 1.0; // 比較係数1
    // インデックスが末尾出ない場合の処理
    if(ctrl_freq_index < FREQ_LEN_DECEL - 1){
      // 非同期モードの周波数を計算
      if(freq < freq_table_decel[ctrl_freq_index + 1][1]){
        ctrl_freq_index++;
      }
      ctrl_freq = freq_table_decel[((unsigned int)ctrl_freq_index) % FREQ_LEN_DECEL][0];
      // リニアに変化する場合の処理
      if(freq_table_decel[((unsigned int)ctrl_freq_index) % FREQ_LEN_ACCEL][2] == 1){
        ctrl_freq += (freq_table_decel[ctrl_freq_index + 1][0] - ctrl_freq) * (freq - freq_table_decel[ctrl_freq_index][1]) / freq_table_decel[ctrl_freq_index + 1][1];
      }
    }
    // 非同期モードの位相を計算
    tri_index += (double)TABLE_LEN * ctrl_freq / (double)CTRL_RATE;
  }
  // 同期モード
  else{
    sync_mode = 1;  // 同期モード
    ctrl_freq_index = 0;  // 非同期モードのインデックスをリセット
    // パルスモードの設定
    if(freq > 80){
      pulse_mode = 0;
    }
    else if(freq > 58){
      pulse_mode = 3;
    }
    else if(freq > 56){
      pulse_mode = 5;
    }
    else if(freq > 44){
      pulse_mode = 7;
    }
    else if(freq > 36){
      pulse_mode = 9;
    }
    else if(freq > 30){
      pulse_mode = 11;
    }
    else if(freq > 26){
      pulse_mode = 13;
    }
    else{
      pulse_mode = 15;
    }
    // 三角波の位相を計算
    tri_index = sin_index * (double)pulse_mode;
    // 比較係数を計算
    if(pulse_mode == 0 || pulse_mode == 1){
      comp_const = 1.0;
    }
    else{
      comp_const = (double)((int)pulse_mode / 2) / (double)((int)pulse_mode / 2 + 1); // 比較係数
    }
  }

  // 位相が1周したら1周分を引く
  while(tri_index >= TABLE_LEN){
    tri_index -= (double)TABLE_LEN;
  }

  // 正弦波が三角波より大きいときは出力ON（U相）
  if(sin_table[((unsigned int)sin_index) % TABLE_LEN] * voltage > tri_table[((unsigned int)tri_index) % TABLE_LEN] * comp_const){
    U_IN_FLAG = HIGH;
  }
  else{
    U_IN_FLAG = LOW;
  }

  // 正弦波が三角波より大きいときは出力ON（V相）
  if(sin_table[((unsigned int)sin_index + TABLE_LEN - TABLE_LEN / 3) % TABLE_LEN] * voltage > tri_table[((unsigned int)tri_index) % TABLE_LEN] * comp_const){
    V_IN_FLAG = HIGH;
  }
  else{
    V_IN_FLAG = LOW;
  }

  // 正弦波が三角波より大きいときは出力ON（W相）
  if(sin_table[((unsigned int)sin_index + TABLE_LEN - TABLE_LEN * 2 / 3) % TABLE_LEN] * voltage > tri_table[((unsigned int)tri_index) % TABLE_LEN] * comp_const){
    W_IN_FLAG = HIGH;
  }
  else{
    W_IN_FLAG = LOW;
  }

  // 周波数が1Hzより大きくまたは加速しているときは出力ON
  if(accel > 0 || freq > 1.0){
    digitalWrite(U_PIN_SD, HIGH);
    digitalWrite(V_PIN_SD, HIGH);
    digitalWrite(W_PIN_SD, HIGH);
    digitalWrite(U_PIN_IN, U_IN_FLAG);
    digitalWrite(V_PIN_IN, V_IN_FLAG);
    digitalWrite(W_PIN_IN, W_IN_FLAG);
  }
  // 周波数1Hz以下で停止または減速しているときは停止
  else{
    // リセット処理
    sin_index = 0;
    tri_index = 0;
    freq = 0;
    ctrl_freq = 0;
    ctrl_freq_index = 0;
    voltage = 0;
    sync_mode = 0;
    digitalWrite(U_PIN_SD, LOW);
    digitalWrite(V_PIN_SD, LOW);
    digitalWrite(W_PIN_SD, LOW);
    digitalWrite(U_PIN_IN, LOW);
    digitalWrite(V_PIN_IN, LOW);
    digitalWrite(W_PIN_IN, LOW);
  }
}

void setup() {
  // ウォッチドッグタイマ無効
  disableCore0WDT();
  disableCore1WDT();
  // 出力モードに設定
  pinMode(U_PIN_IN, OUTPUT);
  pinMode(V_PIN_IN, OUTPUT);
  pinMode(W_PIN_IN, OUTPUT);
  pinMode(U_PIN_SD, OUTPUT);
  pinMode(V_PIN_SD, OUTPUT);
  pinMode(W_PIN_SD, OUTPUT);
  // タイマー割り込みの設定
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000 / CTRL_RATE, true);
  timerAlarmEnable(timer);
}

void loop() {
  // 加速制御
  int acl_val = analogRead(32);
  if(acl_val > 4096.0 * 0.85){
    accel = 5;
  }
  else if(acl_val > 4096.0 * 0.7){
    accel = 3;
  }
  else if(acl_val > 4096.0 * 0.6){
    accel = 1.5;
  }
  else if(acl_val > 4096.0 * 0.3){
    accel = 0;
  }
  else if(acl_val > 4096.0 * 0.25){
    accel = -1;
  }
  else if(acl_val > 4096.0 * 0.2){
    accel = -2;
  }
  else if(acl_val > 4096.0 * 0.15){
    accel = -3;
  }
  else if(acl_val > 4096.0 * 0.1){
    accel = -4;
  }
  else{
    accel = -5;
  }
}
