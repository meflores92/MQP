function [s, w, t, ps, waveform] = operation(rxWaveform, dmrsRNTI, scramblingRNTI,UERNTI,slot,LRB, RBStart)

%Set Varible size and type 
dcibits =  cast(zeros(39, 1), 'int8');
mask = cast(0, 'uint32');
nVar = cast(0.0, 'single');
IPhase = cast(zeros(216,1), 'single');
QPhase = cast(zeros(216,1), 'single');

%Load RNTI fingerpritning Table
RNTI_TABLE = coder.load('RNTI_TABLE', 'Table');
RNTI_TABLE = RNTI_TABLE.Table;

%Function calls for PDCCH Decoding and UE identification
[dcibits,isValid, IPhase, QPhase] = PDCCHCapture(rxWaveform(1:1668), dmrsRNTI, scramblingRNTI, UERNTI, slot);
[dcibits,mask,isValid, IPhase, QPhase] = PDCCHFind(rxWaveform(1:1668), RNTI_TABLE, slot);

%Function call to calculate SNR
SNR = getSNR(rxWaveform(1:1668));

%Function calls for frame syncronization
offset1 = SyncFrameInit(rxWaveform(1:921600));
offset2 = SyncFrame(rxWaveform(1:6584));

%Parameters for spectrogram data
fs = 46.08e6;
nfft = 1536;
s = complex(zeros(1536,600));
w = complex(zeros(1536,1));
t = complex(zeros(1,600));
ps = complex(zeros(1536,600));
coder.varsize('s', [1536 600], [0 0]);
coder.varsize('w', [1536 1], [0 0]);
coder.varsize('t', [1 600], [0 0]);
coder.varsize('ps', [1536 600], [0 0]);

%Function call for getting spectrogram data
[s(1:1536,1:600),w(1:1536),t(1:600),ps(1:1536,1:600)]=spectrogram(rxWaveform(1:921600),ones(nfft,1),0,nfft,'centered',fs,'yaxis','MinThreshold',-130);

%Function call for generating PUSCH jamming signal
waveform = getPUSCH(RBStart, LRB);
end