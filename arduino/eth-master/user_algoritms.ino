byte byteBufferC = 0, // буфер положения ТС
     byteBufferE = 0, // буфер ошибок
     byteBufferS = 0; // буфер состояния датчиков

byte getByteDin1Status() {  // получить состояние д.входа 1 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_m_input & 0b00000001) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

byte getByteDin2Status() {  // получить состояние д.входа 2 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_m_input & 0b00000010) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

byte getByteDin3Status() {  // получить состояние д.входа 3 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_m_input & 0b00000100) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

byte getByteDin4Status() {  // получить состояние д.входа 4 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_m_input & 0b00001000) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

byte getByteDin5Status() {  // получить состояние д.входа 5 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_m_input & 0b00010000) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

byte getByteDin6Status() {  // получить состояние д.входа 6 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_m_input & 0b00100000) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

byte getByteDin7Status() {  // получить состояние д.входа 7 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_m_input & 0b01000000) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

byte getByteDin8Status() {  // получить состояние д.входа 8 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_m_input & 0b10000000) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

byte getByteDin9Status() {  // получить состояние д.входа 9 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_s_input & 0b00000001) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

byte getByteDin10Status() {  // получить состояние д.входа 10 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_s_input & 0b00000010) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

byte getByteDin11Status() {  // получить состояние д.входа 11 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_s_input & 0b00000100) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

byte getByteDin12Status() {  // получить состояние д.входа 12 (1 - есть сигнал, 0 - нет сигнала)
  if ((byte_s_input & 0b00001000) == 0)  {
    return 0;
  } else {
    return 1;
  }
}

void main_algoritm()  {
  
  
}
