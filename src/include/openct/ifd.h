/*
 * Core functions of the IFD handler library
 *
 */

#ifndef IFD_CORE_H
#define IFD_CORE_H

#include <sys/types.h>

typedef struct ifd_apdu {
	unsigned char *		snd_buf;
	unsigned int		snd_len;
	unsigned char *		rcv_buf;
	unsigned int		rcv_len;
} ifd_apdu_t;

typedef struct ifd_device	ifd_device_t;
typedef union ifd_device_params	ifd_device_params_t;

enum {
	IFD_PROTOCOL_DEFAULT = 0,
	IFD_PROTOCOL_T0 = 1,
	IFD_PROTOCOL_T1,
	IFD_PROTOCOL_2WIRE,
	IFD_PROTOCOL_3WIRE,
	IFD_PROTOCOL_I2C,
	IFD_PROTOCOL_TLP,		/* older Gemplus protocool */
	IFD_PROTOCOL_GBP,		/* Gemplus block protocol */

};

typedef struct ifd_protocol	ifd_protocol_t;

typedef struct ifd_driver {
	const char *		name;
	struct ifd_driver_ops *	ops;
} ifd_driver_t;

typedef struct ifd_reader {
	unsigned int		num;
	unsigned int		handle;

	const char *		name;
	unsigned int		nslots;
	unsigned int		flags;

	ifd_device_t *		device;
	ifd_protocol_t *	proto;
	void *			proto_state;
	const ifd_driver_t *	driver;
} ifd_reader_t;

#define IFD_READER_DISPLAY	0x0100
#define IFD_READER_KEYPAD	0x0200

enum {
	IFD_PROTOCOL_RECV_TIMEOUT = 0x0000,

	/* T=0 specific parameters */
	__IFD_PROTOCOL_T0_PARAM_BASE = IFD_PROTOCOL_T0 << 16,

	/* T=1 specific parameters */
	__IFD_PROTOCOL_T1_PARAM_BASE = IFD_PROTOCOL_T1 << 16,
	IFD_PROTOCOL_T1_BLOCKSIZE,
};

enum {
	IFD_DAD_ICC1 = 0,
	IFD_DAD_IFD = 1,
	IFD_DAD_ICC2 = 2,
};

#define IFD_CARD_PRESENT	0x0001
#define IFD_CARD_STATUS_CHANGED	0x0002

extern ifd_reader_t *		ifd_new_serial(const char *, const char *);
extern ifd_reader_t *		ifd_new_usb(const char *, const char *);
extern int			ifd_attach(ifd_reader_t *);
extern void			ifd_detach(ifd_reader_t *);

extern int			ifd_activate(ifd_reader_t *);
extern int			ifd_deactivate(ifd_reader_t *);

extern int			ifd_set_protocol(ifd_reader_t *, int);
extern int			ifd_set_protocol_param(ifd_reader_t *,
					int, long);
extern int			ifd_transceive(ifd_reader_t *, int,
					ifd_apdu_t *);
extern int			ifd_card_status(ifd_reader_t *, int, int *);
extern int			ifd_card_reset(ifd_reader_t *, int,
					void *, size_t);

extern ifd_protocol_t *		ifd_protocol_by_id(int);
extern ifd_protocol_t *		ifd_protocol_by_name(const char *);

extern int			ifd_select_protocol(ifd_reader_t *, const char *);

#endif /* IFD_CORE_H */