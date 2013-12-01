//check if there is a cell above the current position in field.
int checkUpper(uint8_t *field, int position){
  if(position-fieldWidth>0){
    if(field[position-fieldWidth]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
  return 0;
}
//check if there is a cell bellow the current position in field.
int checkLower(uint8_t *field, int position){
  if(position+fieldWidth<fieldSize){
    if(field[position+fieldWidth]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
  return 0;
}
//check if there is a cell to the left of the current position in field.
int checkLeft(uint8_t *field, int position){
  if(position-1>0){
    if(field[position-1]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
  return 0;
}
//check if there is a cell to the right of the current position in field.
int checkRight(uint8_t *field, int position){
  if(position+1<fieldSize){
    if(field[position+1]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
  return 0;
}
//check if there is a cell to the upper left of the current position in field.
int checkUpperLeft(uint8_t *field, int position){
  if(position-fieldWidth+1>0){
    if(field[position-fieldWidth+1]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
  return 0;
}
//check if there is a cell to the upper right of the current position in field.
int checkUpperRight(uint8_t *field, int position){
  if(position-fieldWidth-1>0){
    if(field[position-fieldWidth-1]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
  return 0;
}
//check if there is a cell to the lower Left of the current position in field.
int checkLowerLeft(uint8_t *field, int position){
  if(position+fieldWidth-1<fieldSize){
    if(field[position+fieldWidth-1]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
  return 0;
}
//check if there is a cell to the lower Right of the current position in field.
int checkLowerRight(uint8_t *field, int position){
  if(position+fieldWidth+1<fieldSize){
    if(field[position+fieldWidth+1]){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
  return 0;
}
//return how many lifing cels are around a position in field.
int totalAround(uint8_t *field, int position){
  int around = checkUpper(field, position)+checkLower(field, position)+checkLeft(field, position)+checkRight(field, position)+checkUpperLeft(field, position)+checkUpperRight(field, position)+checkLowerLeft(field, position)+checkLowerRight(field, position);
  return around;
}
