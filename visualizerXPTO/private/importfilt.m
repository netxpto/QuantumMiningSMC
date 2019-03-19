function varargout = importfilt(action,varargin)
%IMPORTFILT  Import function for Filter Structures in the SPTool.
%    'version' - returns string indicating version number for exporting
%      data of this type from SPTool.
%    'make' - creates a new object
%      [err,errstr,struct] = importfilt('make',{input values})
%    'valid' - test if input struc is a valid filter
%    'fields' - return information about the input fields and field names for
%       this type of object in the import dialog.
%     [popupString,fields,FsFlag,defaultLabel] = import*('fields')
%     struct = importfilt('changeFs',struct,Fs) - change the sampling frequency
%             of struct
%     newstruct = importfilt('merge',struct1,struct2) - merge struct1 and struct2
%            into new struct, retaining client specific fields of struct1 and
%            global fields of struct2

%   Copyright 1988-2009 The MathWorks, Inc.

switch action

case 'version'
% versionString = import*('version')
%   returns the version of this component (a string).  This will be saved in
%   any exported SPT data of this type under the SPTIdentifier field
%   of the structure:
%      .SPTIdentifier.version
%      .SPTIdentifier.type  <-- obtained from popupString in the 'fields' call.
   
  varargout{1} = '2.0';


case 'fields'

  PopupString = getString(message('signal:sptoolgui:Filter'));
  
  flds(1).form = getString(message('signal:sptoolgui:TransferFunction'));
  flds(1).fields = {getString(message('signal:sptoolgui:Numerator')) getString(message('signal:sptoolgui:Denominator'))};
  flds(1).formTag = 'tf';
  flds(2).form = getString(message('signal:sptoolgui:StateSpace'));
  flds(2).fields = {['A ' getString(message('signal:sptoolgui:Matrix'))] ['B ' getString(message('signal:sptoolgui:Matrix'))] ['C ' getString(message('signal:sptoolgui:Matrix'))] ['D ' getString(message('signal:sptoolgui:Matrix'))]};
  flds(2).formTag = 'ss';
  flds(3).form = [getString(message('signal:sptoolgui:Zeros')) ', ' getString(message('signal:sptoolgui:Poles')) ', ' getString(message('signal:sptoolgui:Gain'))];
  flds(3).fields = {getString(message('signal:sptoolgui:Zeros')) getString(message('signal:sptoolgui:Poles')) getString(message('signal:sptoolgui:Gain'))};
  flds(3).formTag = 'zpk';
  flds(4).form = getString(message('signal:sptoolgui:OrderSections'));
  flds(4).fields = {getString(message('signal:sptoolgui:SOSMatrix'))};
  flds(4).formTag = 'sos';
  
  FsFlag = 1;
  defaultLabel = 'filt';
  
  varargout = {PopupString,flds,FsFlag,defaultLabel};
  
case 'make'
% [err,errstr,struc, errObj] = importsig('make',{input values})
% Inputs:
%   {input values} cell array -
%     first element is integer giving the value of the 'form' popup
%      (e.g. 1==TF, 2==SS, 3=ZPK, etc)
%     elements 2..N are MATLAB variables.  If FsFlag == 1, the last one
%       is the sampling frequency Fs.
%  This needs to be the only place you use to create new objects,
%  since the fields and their order have to be the same for every object!

    errstr = '';
    errObj = [];
    
    % initialize empty structure:
    struc.tf = [];
    struc.ss = [];
    struc.zpk = [];
    struc.sos = [];
    struc.imp = [];
    struc.step = [];
    struc.t = [];
    struc.H = [];
    struc.G = [];
    struc.f = [];
    struc.specs = [];
    struc.Fs = varargin{1}{end};
    
    switch lower(varargin{1}{1})
    case {'tf',1} % TF
        struc.tf.num = varargin{1}{2};
        struc.tf.den = varargin{1}{3};
    case {'ss',2} % SS
        struc.ss.a = varargin{1}{2};
        struc.ss.b = varargin{1}{3};
        struc.ss.c = varargin{1}{4};
        struc.ss.d = varargin{1}{5};
        struc.tf.num = [];
        struc.tf.den = [];
        try
            [struc.tf.num, struc.tf.den] = ...
                ss2tf(struc.ss.a,struc.ss.b,struc.ss.c,struc.ss.d);
        catch ME
            errObj = message('signal:importfilt:CannotConvertToTF','A,B,C,D');
            errstr = getString(errObj);
        end
        
        if ~isempty(errstr)
            if ~isnumeric(struc.ss.a) || ~isnumeric(struc.ss.b) || ~isnumeric(struc.ss.c) || ~isnumeric(struc.ss.d)
              errObj = message('signal:importfilt:MustBeNumeric','A,B,C,D');
              errstr = getString(errObj);
            end
        end
    case {'zpk',3}  % ZPK
        struc.zpk.z = varargin{1}{2};
        struc.zpk.p = varargin{1}{3};
        struc.zpk.k = varargin{1}{4};
        struc.tf.num = [];
        struc.tf.den = [];
        try
            struc.tf.num = poly(struc.zpk.z(:))*struc.zpk.k;
            struc.tf.den = poly(struc.zpk.p(:));
        catch ME
            errObj = message('signal:importfilt:CannotConvertToTF','Z,P,K');
            errstr = getString(errObj);          
        end
    case {'sos',4} % SOS
        struc.sos = varargin{1}{2};
        struc.tf.num = [];
        struc.tf.den = [];
        try
            [struc.tf.num,struc.tf.den]= sos2tf(struc.sos);
        catch ME
            errObj = message('signal:importfilt:CannotConvertToTF','SOS');
            errstr = getString(errObj);                    
        end
    end

%     Validating the fields for the filter structure
    if isempty(errstr) 
        if ~isnumeric(struc.tf.num) || ~isnumeric(struc.tf.den)
            errObj = message('signal:importfilt:NumDenMustBeNumeric');
            errstr = getString(errObj);                      
        else
            % Force filter numerator and denominator to be rows.
            struc.tf.num = struc.tf.num(:).';
            struc.tf.den = struc.tf.den(:).';
        end
    end
    
    if isempty(errstr)
        if ~all(size(struc.Fs)==1) | ~isreal(struc.Fs) | struc.Fs<=0
            errObj = message('signal:importfilt:FsMustBeScalar');
            errstr = getString(errObj);                                
        elseif isempty(struc.tf.num) | isempty(struc.tf.den) | ...
               norm(struc.tf.num)==0 | norm(struc.tf.den)==0
            errObj = message('signal:importfilt:FilterMustBeNonTrivial');
            errstr = getString(errObj);                                             
        elseif struc.tf.den(1) == 0
            errObj = message('signal:importfilt:FirstDenCoeffMustBeNonZero');
            errstr = getString(errObj);                                                       
        end
    end
    
    % --------
    if isempty(errstr), err = 0; else err = 1; end
  
    if ~err
        struc = importfilt('assignType',struc);
        struc.lineinfo = [];
        struc.SPTIdentifier.type = 'Filter';
        struc.SPTIdentifier.version = importfilt('version');
    else
        struc = [];
    end
    
    struc.label = '';
    
    varargout = {err,errstr,struc,errObj};

case 'valid'
%  [valid,struc] = import*('valid',struc) - return 
%    1 if the input structure is a valid "object", 0 if not.
%    input struc assumed to have field SPTIdentifier with fields
%        .type, .version, and struc.SPTIdentifier.type is the
%        correct string
%    Updates the structure if the version is old.
%    Also sets '.type' field of struc
%  Criteria for validity:
%    - structure has the correct fields in the correct order
%    - some of the fields are checked for class information
    
    valid = 1;   % assume valid on input
    struc = varargin{1};
    
    % first test to see if struc can be added to structure array;
    % if successful, fields are same and in same order
    [err,errstr,vanillaStruc]=importfilt('make',{1 1 1});
%---------------------------------------------------------------------
% FDATool - this don't seem to be necessary, remove for fdatool import.
%---------------------------------------------------------------------
    if ~isfield(struc, 'FDAspecs')
        eval('vanillaStruc(2)=struc;','valid=0;')
    end
    
    % now assuming the fields are all there, test their contents
    % for validity:
    if valid
        valid = isequal(class(struc.tf.num),'double');
    end
    if valid
        valid = isequal(class(struc.tf.den),'double');
    end
    if valid
        valid = (isequal(class(struc.Fs),'double') & length(struc.Fs) == 1);
    end
        
    if valid
        if strcmp(struc.SPTIdentifier.version,'1.0')
            struc.specs = fdutil('updateSpecStruc',struc.specs);
            struc.SPTIdentifier.version = '2.0';
        end
    end

    if valid
        struc = importfilt('assignType',struc);
    end
    
    varargout = {valid,struc};
    return

case 'assignType'
% struc = import*('assignType',struc)

  struc = varargin{1};
  if isempty(struc.specs)
      struc.type = 'imported';
  else
      if isempty(struc.specs.currentModule) | ...
         ~strcmp(struc.specs.currentModule,'fdpzedit')
          struc.type = 'design';
      else
          struc.type = 'imported';
      end
  end
  varargout{1} = struc;
%----------------------------------------------------------------------------
%     struct = importfilt('changeFs',struct,Fs) - change the sampling frequency
%             of struct
case 'changeFs'
    struc = varargin{1};
    newFs = varargin{2};
    oldFs = struc.Fs;
    if ~isempty(struc.f)
        struc.f = struc.f*newFs/oldFs;
    end
    struc.Fs = newFs;
    varargout{1} = struc;
    
%----------------------------------------------------------------------------
%     newstruct = importfilt('merge',struct1,struct2) - merge struct1 and struct2
%            into new struct, retaining client specific fields of struct1 and
%            global fields of struct2
%     This callback is provided for when you import a new structure via
%     the import dialog and there is already a structure in the SPTool that
%     has this name.  Some of the fields (eg, 'data' and 'Fs')  
%     are global and need to be changed by this action, others which are 
%     controlled by the clients (eg, 'lineinfo', 'specs' and 'label') may not.
%
%     Both are assumed valid structures on input.
case 'merge'
    struct1 = varargin{1};
    struct2 = varargin{2};
  
    newstruct = struct2;

    if isempty(struct2.lineinfo)
        newstruct.lineinfo = struct1.lineinfo;
    end
    if isempty(struct2.specs)
        newstruct.specs = struct1.specs;
    end
       
    varargout{1} = newstruct;
end
