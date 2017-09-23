const int SERIAL_BAUD = 115200;

int Mul(short left, short right);
int Add(int left, int right);
int Sub(int left, int right);
short Div(short dividend, short divisor, int &remainder);
int Two(int left);


void setup() {

Serial.begin(SERIAL_BAUD);
}

void loop() {

int opl;
char op;
int opr;
int result;
int remainder;

if (Serial.available() > 0) {
String input = Serial.readString();
sscanf(input.c_str(), "%d %c %d", &opl, &op, &opr);
switch (op) {
case 'x':
result = Mul(opl, opr);
break;
case '+':
result = Add(opl, opr);
break;
case '-':
result = Sub(opl, opr);
break;
case '/':
Serial.print(opl);
Serial.print(" ");
Serial.print(op);
Serial.print(" ");
Serial.print(opr);
Serial.print(" = " );
Serial.print(Div(opl, opr, remainder));
Serial.print(" R ");
Serial.println(remainder);
return;
default:
Serial.println("Usage: left op right\n");
Serial.println("Where op is:");
Serial.println(" + - Add\n - - Subtract\n x - Multiply\n / - Divide");
return;
}
Serial.print(opl);
Serial.print(" ");
Serial.print(op);
Serial.print(" ");
Serial.print(opr);
Serial.print(" = " );
Serial.println(result);
}
}



int Mul(short left, short right) {
int product = 0;
short pos_num;
short pos_num2;
int temp_prod;
int sCounter=0;

int mask = 1 << 16;
if ((left & mask) != 0){
left = Two(left);
sCounter += 1;
}

if ((right & mask) != 0){
right = Two(right);
sCounter += 1;
}

for (int i = 0; i < 16; i++){
if ((1 & left) != 0){
product = Add(product,right);
}

right <<= 1;
left >>= 1;

//right = one being investigated with mask
//left = one being added to total
}
if(sCounter == 1){
product = Two(product);
}
return product;

}

int Add(int left, int right){

int carry = left;
int sum = right;
int nextcarry;
int nextsum;

while (carry != 0){

nextcarry = sum & carry;
nextsum = sum ^ carry;
nextcarry <<= 1;
carry = nextcarry;
sum = nextsum;
}

return sum;

}


int Sub(int left, int right){
int subtracted = Add(left, Two(right));
return subtracted;
}

short Div(short dividend, short divisor, int &remainder){
int mask = 1 << 16;
int sCounter = 0;

if ((dividend & mask) != 0){
dividend = Two(dividend);
sCounter += 1;
}

if ((divisor & mask) != 0){
divisor = Two(divisor);
sCounter += 1;
}

int div1 = divisor << 16;
int quotient = 0;
int x;
remainder = dividend;
for (int i = 0; i < 17; i++){
remainder = Sub(remainder,div1);
if (remainder >= 0){
quotient = ((quotient <<= 1) | 1);
}
if (remainder < 0){
remainder = Add(remainder, div1);
quotient <<= 1;
}
div1 >>= 1;
}
if(sCounter == 1){
quotient = Two(quotient);
}


return quotient;

}

int Two(int left){
int twos1 = ~left;
int final_twos = Add(twos1, 1);
return final_twos;
}
