function userchange(obj)
%UserChange  Method for FD meas object

%   Author: T. Krauss
%   Copyright 1988-2008 The MathWorks, Inc.

obj = struct(obj);
objud = get(obj.h,'userdata');

switch get(obj.h,'style')
case 'edit'
    str = get(obj.h,'string');
    [val,errstr] = fdutil('fdvalidstr',str,objud.complex,objud.integer,...
                              objud.range,objud.inclusive);
    if isempty(errstr)
        objud.lastvalue = objud.value;
        objud.value = val;
        set(obj.h,'userdata',objud)
        set(obj.h,'string',fdutil('formattedstring',obj))
        if ~isempty(objud.callback)
            try
                evalin('base',objud.callback);
            catch ME
                errstr = sprintf([getString(message('signal:sptoolgui:ErrorInEvaluatingCallback')) ':\n%s'],ME.message);
                msgbox(errstr,getString(message('signal:sptoolgui:Error')),'error','modal')
            end
        end
    else
        msgbox(errstr,getString(message('signal:sptoolgui:Error')),'error','modal')
        set(obj.h,'string',fdutil('formattedstring',obj))
    end
    
otherwise
    warning(message('signal:fdmeas:userchange:GUIWarn'))
end
