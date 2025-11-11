int sum(int n);

begin
int x;
read x;
sum(x);
write x;
end

int sum(int n)
begin
int total;
total = 0;
while (n > 0) begin
total = total + n;
n = n - 1;
end
write total;
end