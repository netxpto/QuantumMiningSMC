function [out, t, xmin, xmax, ymin, ymax] = plotSignal(data, symbolPeriod, samplingPeriod, type, numberOfSymbols)

    if length(data)/(symbolPeriod/samplingPeriod) < numberOfSymbols
            numberOfSymbols = length(data)/(symbolPeriod/samplingPeriod);
    end    

    if strcmp(type,'Binary')
        
        out = data(1:numberOfSymbols);
               
        % Change simulation sampling frequency 
        factor = 200; % Upsampling factor
        samplingPeriod = samplingPeriod/factor;
        vect = zeros(1, factor*length(out));
        for k = 1:length(out)
            vect((k-1)*factor + 1:k*factor)= ones(1, factor).*out(k);
        end 
        out = vect;
    else
        out = data;
    end
    
    t = 0:samplingPeriod:numberOfSymbols*symbolPeriod-samplingPeriod;
    out = out(1:length(t));

    if strcmp(type, 'TimeDiscreteAmplitudeContinuousReal')
        stem(t,out);
    end
        
    
    plot(t, out);
    
    xmin = min(t);
    xmax = max(t);
    ymin = min(out);
    ymax = max(out);
    
    if ymin==ymax
        ymax = ymin+1;
        ymin = ymin-1;
    end
    
    
    axis([xmin xmax ymin ymax]);

end