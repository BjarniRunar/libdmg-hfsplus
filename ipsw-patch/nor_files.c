#include "common.h"
#include <xpwn/8900.h>
#include <xpwn/img2.h>
#include <xpwn/img3.h>
#include <xpwn/lzssfile.h>
#include <xpwn/ibootim.h>

AbstractFile* openAbstractFile(AbstractFile* file) {
	uint32_t signatureBE;
	uint32_t signatureLE;

	if(!file)
		return NULL;
	
	file->seek(file, 0);
	file->read(file, &signatureBE, sizeof(signatureBE));
	signatureLE = signatureBE;
	FLIPENDIAN(signatureBE);
	FLIPENDIANLE(signatureLE);
	file->seek(file, 0);

	if(signatureBE == SIGNATURE_8900) {
		return openAbstractFile(createAbstractFileFrom8900(file));
	} else if(signatureLE == IMG2_SIGNATURE) {
		return openAbstractFile(createAbstractFileFromImg2(file));
	} else if(signatureLE == IMG3_SIGNATURE) {
		return openAbstractFile(createAbstractFileFromImg3(file));
	} else if(signatureBE == COMP_SIGNATURE) {
		return openAbstractFile(createAbstractFileFromComp(file));
	} else if(signatureBE == IBOOTIM_SIG_UINT) {
		return openAbstractFile(createAbstractFileFromIBootIM(file));
	} else {
		return file;
	}
}

AbstractFile* duplicateAbstractFile(AbstractFile* file, AbstractFile* backing) {
	uint32_t signatureBE;
	uint32_t signatureLE;
	AbstractFile* orig;
	
	file->seek(file, 0);
	file->read(file, &signatureBE, sizeof(signatureBE));
	signatureLE = signatureBE;
	FLIPENDIAN(signatureBE);
	FLIPENDIANLE(signatureLE);
	file->seek(file, 0);
	
	if(signatureBE == SIGNATURE_8900) {
		orig = createAbstractFileFrom8900(file);
		return duplicateAbstractFile(orig, duplicate8900File(orig, backing));
	} else if(signatureLE == IMG2_SIGNATURE) {
		orig = createAbstractFileFromImg2(file);
		return duplicateAbstractFile(orig, duplicateImg2File(orig, backing));
	} else if(signatureLE == IMG3_SIGNATURE) {
		orig = createAbstractFileFromImg3(file);
		return duplicateAbstractFile(orig, duplicateImg3File(orig, backing));
	} else if(signatureBE == COMP_SIGNATURE) {
		orig = createAbstractFileFromComp(file);
		return duplicateAbstractFile(orig, duplicateCompFile(orig, backing));
	} else if(signatureBE == IBOOTIM_SIG_UINT) {
		orig = createAbstractFileFromIBootIM(file);
		return duplicateAbstractFile(orig, duplicateIBootIMFile(orig, backing));
	} else {
		file->close(file);
		return backing;
	}
}

AbstractFile* openAbstractFile2(AbstractFile* file, const uint8_t* key, const uint8_t* iv) {
	uint32_t signatureBE;
	uint32_t signatureLE;

	if(!file)
		return NULL;
	
	file->seek(file, 0);
	file->read(file, &signatureBE, sizeof(signatureBE));
	signatureLE = signatureBE;
	FLIPENDIAN(signatureBE);
	FLIPENDIANLE(signatureLE);
	file->seek(file, 0);

	if(signatureBE == SIGNATURE_8900) {
		return openAbstractFile2(createAbstractFileFrom8900(file), key, iv);
	} else if(signatureLE == IMG2_SIGNATURE) {
		return openAbstractFile2(createAbstractFileFromImg2(file), key, iv);
	} else if(signatureLE == IMG3_SIGNATURE) {
		AbstractFile2* img3 = (AbstractFile2*) createAbstractFileFromImg3(file);
		img3->setKey(img3, key, iv);
		return openAbstractFile((AbstractFile*) img3);
	} else if(signatureBE == COMP_SIGNATURE) {
		return openAbstractFile(createAbstractFileFromComp(file));
	} else if(signatureBE == IBOOTIM_SIG_UINT) {
		return openAbstractFile(createAbstractFileFromIBootIM(file));
	} else {
		return file;
	}
}

AbstractFile* duplicateAbstractFileWithCertificate(AbstractFile* file, AbstractFile* backing, AbstractFile* certificate) {
	uint32_t signatureBE;
	uint32_t signatureLE;
	AbstractFile* orig;
	AbstractFile* newFile;
	
	file->seek(file, 0);
	file->read(file, &signatureBE, sizeof(signatureBE));
	signatureLE = signatureBE;
	FLIPENDIAN(signatureBE);
	FLIPENDIANLE(signatureLE);
	file->seek(file, 0);
	
	if(signatureBE == SIGNATURE_8900) {
		orig = createAbstractFileFrom8900(file);
		newFile = duplicate8900File(orig, backing);
		replaceCertificate8900(newFile, certificate);
		return duplicateAbstractFile(orig, newFile);
	} else if(signatureLE == IMG2_SIGNATURE) {
		orig = createAbstractFileFromImg2(file);
		return duplicateAbstractFile(orig, duplicateImg2File(orig, backing));
	} else if(signatureLE == IMG3_SIGNATURE) {
		orig = createAbstractFileFromImg3(file);
		newFile = duplicateImg3File(orig, backing);
		replaceCertificateImg3(newFile, certificate);
		return duplicateAbstractFile(orig, newFile);
	} else if(signatureBE == COMP_SIGNATURE) {
		orig = createAbstractFileFromComp(file);
		return duplicateAbstractFile(orig, duplicateCompFile(orig, backing));
	} else if(signatureBE == IBOOTIM_SIG_UINT) {
		orig = createAbstractFileFromIBootIM(file);
		return duplicateAbstractFile(orig, duplicateIBootIMFile(orig, backing));
	} else {
		file->close(file);
		return backing;
	}
}


void replaceCertificateAbstractFile(AbstractFile* file, AbstractFile* certificate) {
	uint32_t signatureBE;
	uint32_t signatureLE;
	AbstractFile* f;
	
	file->seek(file, 0);
	file->read(file, &signatureBE, sizeof(signatureBE));
	signatureLE = signatureBE;
	FLIPENDIAN(signatureBE);
	FLIPENDIANLE(signatureLE);
	file->seek(file, 0);

	if(signatureBE == SIGNATURE_8900) {
		f = createAbstractFileFrom8900(file);
		replaceCertificate8900(f, certificate);
		f->close(f);
	} else if(signatureLE == IMG3_SIGNATURE) {
		f = createAbstractFileFromImg3(file);
		replaceCertificateImg3(f, certificate);
		f->close(f);
	}
}

