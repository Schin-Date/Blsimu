#! /usr/bin/env python3
###! /usr/local/bin/python3
#_*_ coding: utf-8 _*_
###import codecs
###sys.stdout = codecs.getwriter('utf_8')(sys.stdout)
import sys
import ast
from datetime import datetime
#

class ReadDoPrm:
    
    #definitions of class variables
    _printLvl=0
    #sdataPair_lst = [] # = [(tag1, val1), (tag2, val2),...]
    data_lst = [] # = [(tag1, type1, val1), (tag2, type2, val2),...]
    data_type_tags = ('int', 'double', 'string', 'str', 'bool', 'UInt_t', 'Bool_t', 'Double_t', 'Int_t')
    
#1    def __init__(self,in_txtFile, out_logFile):
#1        #if 1< printLvl: print('readData::init')
    def __init__(self):
        print('N...ReadDoPrm:: __init__: called...')
    #-----------------------------
#1        self.txt2sdataPair(in_txtFile, out_logFile)
    #-----------------------------
    #parsing: parse, parser
#=================================================================================
    def txt2dataLst(self,in_txtFile='',out_logFile=''):
        in_txt_l = []
        if ''!= in_txtFile:
            import os.path
            if True==os.path.isfile(in_txtFile):
                with open(in_txtFile,'r') as infile:
                    for line in infile:
                        in_txt_l.append(line.strip())
                infile.close()
            else:
                print('E...ReadData::txt2data: File {} not found.'.format(in_txtFile))

        if ''==out_logFile: fLog = sys.stdout
        else:
            fLog = open(out_logFile,'w')
        fLog.write('date: '+str(datetime.today())+'\n')
        
        fLog.write('\nDataRead LOG\n==============\n')
        #iline=0
        for iline, line in enumerate(in_txt_l):
            
            logline_lst =['{}:{}'.format(iline,line)]
            
            #omit empty lines and lines starting with // or #
            if 1>len(line) or '//'==line[0:2] or '#'==line[0:1]:
                logline_lst.append(' ........ignored\n')
                self._printLog(logline_lst,fLog)
                continue
            else:
                logline_lst.append('\n')
            
            self._printLog(logline_lst,fLog)

            #omit part after //, /*, # or ;
            line_valid_s=line.split('//')[0].strip() #omit after //
            line_valid_s=line_valid_s.split('/*')[0].strip() #omit after /*
            line_valid_s=line_valid_s.split('#')[0].strip() # omit after #
            line_valid_s=line_valid_s.split(';')[0].strip() # omit after ;

            #replace ',' by ' '
            line_valid_s = line_valid_s.replace(',',' ')

            if line != line_valid_s:
                logline_lst =['   {}\n'.format(line_valid_s)]
                self._printLog(logline_lst,fLog)
                    
            if 'enddoprm' in line_valid_s:
                break

            words = line_valid_s.split()
            numwords = len(words)

#-------------------------------------------------------------- Parsing
            vartype_s = ''
            if words[0] in self.data_type_tags:
                vartype_s = words.pop(0)
                line_valid_s = " ".join(words)
                logline_lst =['      "{}"\n'.format(line_valid_s)]
                self._printLog(logline_lst,fLog)
            if '=' in words:
                pos_equal = words.index('=')
                if 1 != pos_equal:
                    print('E...readDoPrm::txt2data:pos_equat={}'.
                          format(pos_equal))
                    break
                del words[1]
                line_valid_s = " ".join(words)
                logline_lst =['         "{}"\n'.format(line_valid_s)]
                self._printLog(logline_lst,fLog)
            
            varName = words.pop(0)
            rightValue_s = " ".join(words)
            
            # check varName and rightValue_s meet the input format here
            # REWRITE the part below to protect this code
            try:
                #valD = ast.literal_eval(rightValue_s) # more safe but can not parce e.g. "LPNAM + 1"
                valD = eval(rightValue_s) #<<<<<<<<<<<< this is NOT SECURE!! <<<< BE AWARE
            except:
                print('E...readDoPrm::txt2data: rightValue_s : {}\n'.format(rightValue_s))
                break
            
            vartype_tag = ''
            if '' != vartype_s:
                if 'string' == vartype_s or 'str' == vartype_s:
                    vartype_tag = 'str'
                    py_val = str(valD)

                elif 'double' == vartype_s or 'Double_t' == vartype_s:
                    vartype_tag = 'double'
                    py_val = float(valD)
                
                elif 'int' == vartype_s or 'Int_t' == vartype_s or 'UInt_t' == vartype_s:
                    vartype_tag = 'int'
                    py_val = int(valD)
                
                elif 'bool' == vartype_s or 'Bool_t' == vartype_s:
                    vartype_tag = 'bool'
                    py_val = bool(valD)

                else:
                    logline = "E...readDoPrm::txt2data: Unrecognizede type vartype_s = '{}'\n".\
                            format(vartype_s)
                    #logline = "E...DDD {}\n".format(vartype_s)
                    logline_lst.append(logline)
                    print(logline)
                    self._printLog(logline_lst,fLog)
                    exit(9)
            else:
                vartype = type(valD)
                if vartype is str:
                    vartype_tag = 'str'
                elif vartype is float:
                    vartype_tag = 'double'
                elif vartype is int:
                    vartype_tag = 'int'
                elif vartype is bool:
                    vartype_tag = 'bool'
                else:
                    logline = "E...readDoPrm::txt2data: Unexpected py_type vartype = '{}'\n".\
                        format(vartype)
                    logline_lst.append(logline)
                    print(logline)
                    self._printLog(logline_lst,fLog)
                    exit
                py_val = valD


            if 'str' == vartype_tag:
                macro_l = '{}="{}"'.format(varName,py_val)
            else:
                macro_l = "{}={}".format(varName,py_val)
            print('D...macro_l : ',macro_l)
            exec(macro_l)
            #+++++++++++++++++
            #<varName> is a variable taking a value <py_val> in the Python interpreter
            
            #  Though py_val have correct types here,
            # we bring them back to str to use them in C++ templated method
            # to convert them into C types.
            py_val = str(py_val)
            #+++++++++++++++++++++

            #self.sdataPair_lst.append([varName,rightValue_s])
            self.data_lst.append([varName, vartype_tag, py_val])
    
    
        for i, data in enumerate(self.data_lst):
            print('{}: {} ({}) {}'.
                  format(i, data[0], data[1], data[2]))

        return self.data_lst


    def _printLog(self,line_lst,fout):
        for line in line_lst:
            fout.write(line)





