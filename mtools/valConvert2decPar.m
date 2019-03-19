function [val_res]=valConvert2decPar(S,nFracbit,nbit)

% nFracbit = 6;
% nbit = 8;
val_res = zeros(1,length(S));
for m=1:length(S)
    if S(m)>((2^nbit)/2)
        val_res(m) = (S(m)-(2^nbit))/2^nFracbit;
    else
        val_res(m) = S(m)/2^nFracbit;
    end   
end