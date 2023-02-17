bin_PROGRAMS = gusto
gusto_SOURCES = \
		defs.h              \
		main.h              \
		user_data.h         \
		version.h           \
		gusto.c             \
		callbacks.c         \
		css.c               \
		convert.c           \
		main_ui.c           \
		utility.c     

gusto_CFLAGS = $(X_CFLAGS) -Wno-deprecated-declarations
gusto_LDADD = $(X_LIBS) -ljpeg -lpthread
