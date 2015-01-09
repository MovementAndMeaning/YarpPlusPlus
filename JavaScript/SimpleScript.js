//--------------------------------------------------------------------------------------------------
//
//  File:       SimpleScript.js
//
//  Project:    M+M
//
//  Contains:   An example script that meets the requirements for a JavaScript file to be used with
//              the JavaScript input / output service.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2015 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and / or
//                  other materials provided with the distribution.
//                * Neither the name of the copyright holders nor the names of its contributors may
//                  be used to endorse or promote products derived from this software without
//                  specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//              EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//              OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//              SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//              INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//              TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//              BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//              ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//              DAMAGE.
//
//  Created:    2015-01-06
//
//--------------------------------------------------------------------------------------------------

// Some test JavaScript...

writeStringToStdout('hello ' + 'world, it is ' + new Date());

var ww = 4.34, xx = 5;

function hereWeGo()
{
    writeStringToStdout('inside hereWeGo');
}

var yy = {};

var zz = { aa : 'first', bb : 'second', cc : 42};

function andAnotherFunction(aa)
{
    writeStringToStdout('argument is ' + aa);
    hereWeGo();
}

andAnotherFunction(42);
writeStringToStdout('argv is: [' + argv.toString() + ']');

// The real stuff:

function scriptHandleInput(portNumber, incomingData)
{
    writeStringToStdout('input on port ' + portNumber);
}

var scriptDescription = 'An example script';

var scriptInlets = [ { name: 'incoming', protocol: 'd+',
                    protocolDescription: 'A sequence of floating-point values',
                    handler: scriptHandleInput } ];

var scriptOutlets = [ { name: 'outgoing', protocol: 'i+',
                        protocolDescription: 'A sequence of integer values' } ];
