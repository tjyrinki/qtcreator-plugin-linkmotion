TEMPLATE = subdirs

SUBDIRS += src tests

OTHER_FILES += %ProjectName%.spec

LM_CHECKS = tests
CONFIG += lm_check
