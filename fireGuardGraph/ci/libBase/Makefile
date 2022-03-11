include $(MAKEROOT)/stdmk
include $(MAKEROOT)/stdmk.ace
include $(MAKEROOT)/stdmk.cop

MODULE_DIR = caf/ci/libBase

LIB_NAME = ciBase
#LIBS =libciBase.a

HDRS = \
		ciMacro.h \
		ciBaseType.h \
		ciListType.h \
		ciWin32DLL.h \
		ciAceType.h \
		ciStringTokenizer.h \
		ciPointer.h \
		ciTime.h \
		ciStringUtil.h \
		ciUtil.h \
		ciErrObject.h \

SRCS = \
		ciStringTokenizer.C \
		ciTime.C \
		ciStringUtil.C \
		ciUtil.C \
		ciErrObject.C \

OBJS =  $(SRCS:.C=.o)

USER_INCLUDES +=
USER_LIBDIR +=
USER_LIBS +=
USER_ENTRIES +=

include $(MAKEROOT)/stdmk.lib
