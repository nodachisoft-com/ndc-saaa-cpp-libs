# 共通設定
PJNAME1=ndc-cpp-libs
PJNAME1_TEST=ndc-cpp-libs-test
PJNAME2=ndc-saaa-cpp-libs
PJNAME2_TEST=ndc-saaa-cpp-libs-test

OUTDIR=temp

# 共通コマンド
COPYDIR=XCOPY /T /E
FIND=C:\01_add\005_dev\020_msys2\tools\msys64\usr\bin\find.exe
DELETE_RECURSIVELY_WIN=del /Q 
DELETE_RECURSIVELY_LINUX=rm -rf 


# コンパイラとコンパイルオプションの定義
CC = cl.exe
#CFLAGS = /c /nologo /EHsc /std:c++17 /JMC /permissive- /GS /W3 /Zc:wchar_t /ZI /Gm- /Od /sdl /Fd"temp\ndc-cpp-libs.pdb" /Zc:inline /fp:precise /D "_DEBUG" /D "_LIB" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /MDd /FC /Fa"temp\"  /Fo"temp\" /Fp"temp\ndc-cpp-libs.pch" /diagnostics:column 
# CFLAGS = /c /nologo /EHsc /std:c++17 /JMC /permissive- /GS /W3 /Zc:wchar_t /ZI /Gm- /Od /sdl /Fd"temp\\ndc-cpp-libs.pdb" /Zc:inline /fp:precise /D "_DEBUG" /D "_LIB" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /MDd /FC /Fa"temp\\"  /Fp"temp\\ndc-cpp-libs.pch" /diagnostics:column 
CFLAGS = /c /nologo /EHsc /std:c++17 

# リンカとリンクオプションの定義( Windows 用 )
LIB = C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.35.32215\bin\Hostx86\x86\lib.exe
LINK = C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.35.32215\bin\Hostx86\x86\link.exe
LINKPRAM = /subsystem:console /nologo /DEBUG /out:"temp\\ndc-cpp-libs.lib"

SRCDIRS_PJ = ndc-cpp-libs
OBJDIR = temp

SRCEXT = cpp
OBJEXT = obj

OBJROOT=temp
SRCROOT=.

SRCDIRS = $(shell $(FIND) $(SRCDIRS_PJ) -type d)
SOURCES := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.$(SRCEXT)))

OBJSRCS = $(SOURCES:$(SRCROOT)/%=%)
OBJECTS = $(addprefix $(OBJROOT)/, $(OBJSRCS:.cpp=.obj))

#SOURCES := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.$(SRCEXT)))
#OBJECTS := $(patsubst %.$(SRCEXT),$(OBJDIR)/%.$(OBJEXT),$(SOURCES))

all: ndc-cpp-libs.lib

#$(OBJDIR)/%.$(OBJEXT): %.$(SRCEXT)
$(OBJROOT)/%.obj : $(SRCROOT)/%.cpp
	@echo "Compiling $< ..."
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $< /Fo$@

ndc-cpp-libs.lib: $(OBJECTS)
	@echo "Linking $@ ..."
	$(LIB) /nologo /out:temp/ndc-cpp-libs.lib $^

clean:
	@echo "Cleaning up ..."
	@$(DELETE_RECURSIVELY_LINUX) ./$(OBJDIR)/*.$(OBJEXT)
	@$(DELETE_RECURSIVELY_LINUX) ./$(OBJDIR)/ndc-cpp-libs.lib
	@$(DELETE_RECURSIVELY_LINUX) $(OBJDIR)

debug:
	@echo $(OBJECTS)


makedir:
	@mkdir -p $(OUTDIR)
	$(COPYDIR) $(PJNAME1) $(OUTDIR)
	$(COPYDIR) $(PJNAME1_TEST) $(OUTDIR)
	$(COPYDIR) $(PJNAME2) $(OUTDIR)
	$(COPYDIR) $(PJNAME2_TEST) $(OUTDIR)



