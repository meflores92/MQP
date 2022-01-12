total = [];
for x = 0:255
    value = dec2bin(uint8(x));
    for i = 1:length(value)
        total(x+1,i) = str2num(value(i));
    end
    for i = length(value)+1:8
        total(x+1,i) = 0;
    end
    
end
good = [];
for x = 1:256
    a = xor(total(x,1), total(x,2));
    b = xor(total(x,3), total(x,4));
    c = xor(total(x,5), total(x,6));
    d = xor(total(x,7), total(x,8));
    
    u = xor(a,b);
    g = xor(c,d);
    if (u == g)
        good = [good;total(x,:)];
    end
end
final = [];
for x = 1:256
    count = 0;
    for i = 1:length(good)
        new = xor(total(x,:), good(i,:));
        a = xor(new(1), new(2));
        b = xor(new(3), new(4));
        c = xor(new(5), new(6));
        d = xor(new(7), new(8));
        u = xor(a,b);
        g = xor(c,d);
        if (u == g)
            count = count + 1;
        end
    end
    final = [final, count];
    
end

final