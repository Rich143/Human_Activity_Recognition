/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Fri Apr 11 12:12:14 2025
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_weights_array_u64[1532] = {
  0xbdb72d10beb505cdU, 0xbe2519e33e9e5a19U, 0x3e43a123be361067U, 0xbcac1a233e477abfU,
  0xbe8adce33bb680b0U, 0xbe5e10db3f003d3bU, 0xbec610e9be572a24U, 0xbf06eb55bdbc32adU,
  0x3ebb7444beb0a6c7U, 0x3d840c3fbd493ccdU, 0x3dc15367bf20f27bU, 0x3c00a7b93de55c47U,
  0xbdf4efe8bd3a6928U, 0x3e457ad8bc9f0835U, 0xbaf03ddfbe00a4ecU, 0xbc02f00d3bc87c4bU,
  0x3e6218973e434a24U, 0xbe9743b83e6a245dU, 0xbf14b8b1beaab7d6U, 0x3df1faef3ddca15fU,
  0xbe95528a3e7e49ecU, 0xbdeb90e8bda6e361U, 0x3f348a293e15dfe3U, 0x3da645fa3e3ff152U,
  0xbd236ffa3da72c1fU, 0xbcc6744abe26bc89U, 0xbefaa34b3c9ea69bU, 0xbe8ec9babf8abacfU,
  0xbe33af6ebe9a0e60U, 0x3db0da00be2b49d8U, 0x3d672a78be1528fcU, 0x3e5163a93dc4e9beU,
  0xbdbdda30be2ce399U, 0xbeb241a53e394a9cU, 0x3ca93adabe4a78c8U, 0xbe6d4361bed32da2U,
  0xbf0c6bb83eb761caU, 0xbe330af73efb9ef6U, 0xbedc670abe3291dfU, 0x3e9bc8263e8d4357U,
  0xbe597338bf235955U, 0x3e54474bbdb0e73fU, 0x3e8b3d6e3f0613f8U, 0xbd6de5edbe82cd6cU,
  0x3d929dfcbdd8a86cU, 0xbe43fea4beb65c70U, 0x3e53e852bd3675f0U, 0x3e2e39d63e47b2b4U,
  0xbe6129fb3d7883daU, 0x3f80ca5cbf04f345U, 0x3d39536f3d6613aaU, 0xbeb00dacbe8c08b4U,
  0xbe29732cbe8a6676U, 0x3ea038413d238b8dU, 0x3d20f03d3e1495ceU, 0x3d89ca743d672e6fU,
  0x3ec8f346bf44b0c2U, 0x3f49af3ebee32ae4U, 0x3cb95c49bb54a891U, 0x3e447d0b3d992951U,
  0x3b483bd2bc01a8ebU, 0xbe65163fbe2f0f97U, 0xbe486819be2e8548U, 0xbe8ce582beda9001U,
  0xbe5660c83eadcc69U, 0x3d1bccc63f107e7bU, 0xbdd1da2ebdf90eecU, 0x3ed515c0be662614U,
  0x3ed679ba3de7bee6U, 0x3e8b45a43db5f080U, 0x3ebbfae83ea95fecU, 0xbce3ae293d9e0b7bU,
  0xbd114b2cbe1ade6aU, 0xbe64a0b03e024c2dU, 0x3cc05263be2137e8U, 0xbf10e6c3bea37c56U,
  0xbdb615713f48d1c7U, 0xbe674c21bd1fbbccU, 0xbeb047b63ef23f8dU, 0xbe3689d93e6d0ec2U,
  0x3d612b813e55df6bU, 0xbf1c5f75bec38f03U, 0xbe098ec3bedf26cbU, 0xbe323ee5be2d62ddU,
  0x3df46897be09c35cU, 0x3e2eca623e86ff32U, 0xbe8fe463bbcc88b0U, 0xbe9fe0bebe22b127U,
  0x3c91b4333e4d793bU, 0x3eb160b43e9e098dU, 0x3e9f68a63db7ff83U, 0x3e240f9a3e50bc4dU,
  0xbe34df92bddbb5c8U, 0x3d430c103db17b68U, 0x3dde2c08be034d35U, 0x3f82229ebe86feebU,
  0x3bfdbbb4bf1392bfU, 0xbe1615e13cdbb9f1U, 0xbe45c3b13ed59680U, 0xbe918983be832ec7U,
  0x3dd9bbc1be9b9175U, 0x3ed6784fbcffc8c9U, 0xbde96d2bbeb0cadeU, 0xbf2a6bd33e602aa7U,
  0xbf0b2695beb583acU, 0x3de372bf3ec7a563U, 0x3cef1b58be2d564cU, 0x3d90e6e53da01539U,
  0x3f35afe33eaa71feU, 0x3d890bca3e2d6713U, 0xbe77bc1bbe635bc1U, 0xbe10c4ebbe1e3a49U,
  0x3e4e10673f071fd5U, 0xbe083a923e047635U, 0x3dea8fb33dfd3a1fU, 0x3ef69c3abd5f4ba1U,
  0xbdfb7042bd4dddcfU, 0xbe9221023eba2874U, 0xbf102affbd8fa03dU, 0x3f07abb73dd35459U,
  0x3e650fa7bde5e0c7U, 0x3c1ea5c13e0b64aaU, 0x3dd16f41be51f49cU, 0xbdc0ff043e73960dU,
  0xbe5d32cfbe83c368U, 0xbe2f149abda611faU, 0x3df11a29bea33f90U, 0x3e9a8722becb8179U,
  0x3d2907873cf351c0U, 0x3e20cdbc3dd87060U, 0x3e0ce9283e8ed183U, 0xbebaeee3be92386aU,
  0xbe8c15febedbc1b3U, 0xbb15dd4b3e067da7U, 0x3e2de64cbeb2e846U, 0x3f072a8d3ee5e30cU,
  0xba5d58ce3df46293U, 0xbd56da70bd6c0714U, 0xbea73b53bea25218U, 0xbe56fefbbe9284a2U,
  0xbe71dab2be6f4f56U, 0x3dfae083bf3381b3U, 0x3e9494433e663aeaU, 0x3eb3d6773dec1a71U,
  0xbcd066b5bd8a9eb7U, 0x3ea2c2cc3e0181dcU, 0x3c3d7aefbe43a1e9U, 0x3e0ed2563cd9e343U,
  0xbe0703273d580732U, 0xbda3c3b53e2e6538U, 0x3e6ff89abd016f3bU, 0xbe863297bec654fcU,
  0x3e10100c3baf7892U, 0x3e8c00333d9c21d6U, 0xbd4bc473bcccac7fU, 0xbebd0f96bdfa91e9U,
  0xbefcb7c9bf672f5aU, 0xbcccde05bc55086bU, 0x3d2bbeb33e015d98U, 0x3dd1f4b63bd62115U,
  0xbc45fbf83c4ec18eU, 0xbc83e0493c7c2d59U, 0xbd09ee2ebd2ff8b8U, 0xbc0142aa3cf2baa7U,
  0xbd28f2d7bc944e60U, 0xbca9f5ef3d5dd2f8U, 0x3d97cb5fbc99b1bdU, 0xbb965b61bce02b83U,
  0xbea444c9baecba95U, 0xbe6354fa3e1a518bU, 0xbef054843cd9f8ecU, 0xbd4af35cbdd10feeU,
  0xbe1b70b6be43dea9U, 0xbd1d6f58bcbc4380U, 0x3d89ea233d0527aeU, 0x3e26aec83e06f747U,
  0x3cb5d847bd88b2e8U, 0xbed5dc96bcaab9aeU, 0x3ecfc4c7bd93895fU, 0x3e3fcd573f4718faU,
  0x3d439c7b3f039dc1U, 0x3e2e7111be1b5255U, 0x3f2e26993d12ce91U, 0xbe5bd1f9be1ca21bU,
  0x3c501b78ba85b9cbU, 0xbda7a670bdc80d29U, 0x3e84a59ebde04a8fU, 0x3e4ae4b73e3523b5U,
  0x3e1c60cb3ec93191U, 0xbd1aafa3bca1da43U, 0x3edd9e9d3df7193bU, 0xbf85f918bf027534U,
  0xbfee455ac11c35a6U, 0xc0ba570cc12691bfU, 0xc133d14ac13fbd40U, 0xc12256dac13aa096U,
  0xc1340fe4c133b9b4U, 0xc12077fbc042ff62U, 0xc12ad980c10e0301U, 0xc10c30edc1383959U,
  0xc12b0e33c09c1dc9U, 0xc0ec1620bddfcb50U, 0xbd089bbdc0751293U, 0xc0cbed10c12fc87dU,
  0xbe4e4e643e3ec594U, 0x3e94104ebe479cfaU, 0x3e834df0be251e24U, 0xbeb42471be976d18U,
  0xbf0b250c3f0c14e7U, 0xbce61e88bde88e9dU, 0xbf217d94bdb0bbd3U, 0xbdb0d21e3e7f527aU,
  0xbf2718ebbda6cf8eU, 0x3e188c42bd5bc688U, 0xbe52a2efbed55359U, 0xbcbab2073e826fb0U,
  0x3c0cdc8b3ebc06b2U, 0x3e54eeebbe16ad24U, 0xbf1b8cbc3f2b1f17U, 0xbdf28111bd129846U,
  0x3f0efd453e87102bU, 0xbe80b6723e4c4350U, 0x3cceb2203ec21c57U, 0x3f0fbd87bd817a23U,
  0xbec55df23ebcd2cfU, 0xbf025282be976a6fU, 0xbeedbe7fbf112ce5U, 0xbef25cdfbba672d3U,
  0xbcb099d0beb532a9U, 0xbc5ed5ddbe5f034bU, 0x3eae56403f136ae0U, 0x3f0ca16dbdde72baU,
  0x3f35dcb0bd26f0dfU, 0x3dd68126be8006f8U, 0xbe67010ebceb8341U, 0x3ee3b7cfbe59dc31U,
  0xbea6a47f3e5f1995U, 0x3e45263ebec42592U, 0xbf126a263d22b6d0U, 0xbca81d703cd34365U,
  0xbd4d0577bf3e2b52U, 0x3ea6c770bda6d2d0U, 0xbe3c46c9bf5503a2U, 0xbd98ebeabe03c93bU,
  0xbec1309f3eac18a8U, 0xbca3b6d9bdf38461U, 0x3e5d02193dfc29efU, 0x3f0440073e133022U,
  0x3cb7a63cbe98d81cU, 0x3c8bd5fa3d2a8a4cU, 0x3dd578a33eca49b0U, 0x3e0d3b77bcc49c35U,
  0xbe66e4793eb832e4U, 0x3f10a5c83ea7b0a2U, 0x3e14a2e73ee29b4cU, 0x3eb64dd3be5aa993U,
  0xbebd7124bd08bbc2U, 0xbe145160bddfc30bU, 0x3ebb56f73d759954U, 0x3ea035493ebf08cdU,
  0xbf5049e83df0c7afU, 0x3ddbb649be5d7501U, 0xbdf1b926be9981dfU, 0x3d67347bbe865416U,
  0x3b217d483e465b87U, 0x3e9a5bf1be401ab9U, 0x3e5489793c90545bU, 0x3e986b1e3cbfa18fU,
  0x3f046e85bd639966U, 0xbe2d67d2be51c53cU, 0x3f0899d23f00a1d5U, 0xbe3d470cbebfc8eaU,
  0xbe855c84bd2ec916U, 0xbd6a5123bf53ea3dU, 0xbe7f48a3beb0dfeeU, 0x3bf989923d9d666eU,
  0xbee44bd7be10ebe9U, 0x3e9132e93e506263U, 0x3e9d1e083d7f6a1cU, 0xbdc55ad13e0d26c6U,
  0x3ea59d8f3ec0f3c6U, 0x3d3919e43dae1b80U, 0xbee6e6bcbdecdf23U, 0x3ec21286bf1192c3U,
  0xbec1f6e83d38d6c5U, 0x3d991686bdfd1268U, 0xbed3bfab3d54b898U, 0xbe0e4ef73e5a6d36U,
  0xbe022075bf255cc7U, 0x3e2127b8bf3ba21eU, 0xbe059073bf1ca52dU, 0x3f0f79f4bdae72a1U,
  0x3eb558483d942e57U, 0x3e7870debe0b9607U, 0xbe0b8643bf099a88U, 0x3f610fc5bdb04017U,
  0xbdea9012bf2c385eU, 0x3eb6af22be95a8f8U, 0xbecf17133f09588fU, 0xbeaa9ceebee7dda4U,
  0x3f0adfa43e96bca1U, 0xbccab9013d64513cU, 0x3d74ff873e0ce134U, 0x3e11d7f3be04d964U,
  0x3a5e74ebbd6b6e2bU, 0x3b995e6abf008f28U, 0x3e8816053e33624eU, 0x3f1f1903bd87d731U,
  0x3cf73ca0be48133fU, 0x3de7404cbe9ed01bU, 0xbf00862abe889081U, 0xbd9817d0be8a4b91U,
  0xbe84886cbf16214eU, 0x3defb0673ed18939U, 0x3f38890d3f42b77fU, 0x3da51aa3bec13f9bU,
  0xbe78d0d5be35ca5fU, 0xbe1283c2be16d402U, 0x3f4e63a1bdcc2f5bU, 0x3f38a4bd3ecbd9ecU,
  0x3cad96f4be90e846U, 0x3eaf1f423e0ab9d5U, 0xbc3be4653f1f7290U, 0x3d66124fbe046ec3U,
  0x3cd3376b3ed13b78U, 0x3e053c653ea7359bU, 0xbefdbb283eaa183bU, 0x3d5deb9cbf1737d9U,
  0x3f1563473e173246U, 0xbe903fe3be1637e2U, 0x3f1809b43f31d4a4U, 0xbed6532b3dc506d7U,
  0xbeb91284be64b1deU, 0xbe9b46a03cfa208bU, 0x3de6c593bdfb787cU, 0xbe1dcd123df576ecU,
  0x3e6232243f1a54dbU, 0x3ee23f183f558b04U, 0x3f431b833f2f9a7dU, 0xbf27e71f3f0ff2adU,
  0x3d59f7ef3f397ee6U, 0x3e8a2dbdbe3024ebU, 0x3eb57f1c3e8c56d9U, 0x3f8783b33d1519e2U,
  0xbf74d4e7bf048e1dU, 0x3f0d8bc5be156926U, 0x3e37f5413e088a3bU, 0x3ec2c1ecbf4f7502U,
  0xbd6168733d320460U, 0x3d75ec213f1c0eb6U, 0x3f0998003f282765U, 0x3c343237bf38a85fU,
  0xbe73a5cd3f94dd06U, 0xbea66a22bed6ea7fU, 0x3f0736203e9a3751U, 0x3ee625283f0be132U,
  0xbecf82eebee4affdU, 0x3e48d4e53f01139fU, 0x3f0527323e0b8cbdU, 0xbe1a02e7bd9152d4U,
  0xbc41e7f5bf01beaeU, 0x3ec730823edda2ecU, 0xbf44d98f3e71cc63U, 0x3f0d4699bf139644U,
  0x3ef96e213f0d39dfU, 0xbecafac4bd3ea0a9U, 0x3e3a933c3f4205d9U, 0xbe300a0b3f491e7aU,
  0x3f5ce3363c7c206dU, 0xbdc749483dd18451U, 0x3d395fbe3ba89477U, 0x3d440575bc2b8eb8U,
  0x3d2ce45a3e2f3a31U, 0x3eac5aa23f1356f5U, 0x3f1d4e4c3ef5e3b5U, 0xbe578a543e72384dU,
  0x3edab52f3f638b85U, 0x3f265666bee38924U, 0x3edfebd83d8f05daU, 0x3e9dd0c83ecd7b67U,
  0xbf860557bf0f7e71U, 0x3eda76713c390d81U, 0x3de7a8333ecb4227U, 0x3edf06c9bf4ad8b8U,
  0xbd87203cbe2fdc22U, 0x3dac66193f0e5e88U, 0x3ec101fd3f8636e6U, 0x3cfa379fbf2ac834U,
  0xbd1a9c0bbe164e8dU, 0xbf25bad7be50af26U, 0x3e0c28c53eaa73f8U, 0x3f9155103f2698ddU,
  0x3e7dc23dbe7be646U, 0x3e80cf6b3ee42b3cU, 0x3e1ea1d33d817789U, 0xbdc64b473bfc4336U,
  0xbc89cdc8bee0438fU, 0x3e8e727a3f76b88cU, 0xbeb1f8823f3376a0U, 0x3e2ad97abefb538fU,
  0xbdf9ab8fbe889a67U, 0xbc6d9512be3e7226U, 0x3f257d143f192e60U, 0x3f4503c03e20d552U,
  0xbe734d8bbeb5eb71U, 0x3d31f6c9bc786647U, 0xbdb52b1d3da42dfaU, 0xbdf15e15be8f8712U,
  0x3d582de4be753dffU, 0x3eb4520a3f4e56d8U, 0x3e59e2ee3fa42087U, 0xbeca41823e94fd48U,
  0x3e4945bb3f8622f4U, 0x3f666a5abead6cc0U, 0x3f3d4f5f3eedaec8U, 0xbf04c5153f555d1aU,
  0xbf504b43bf39cf21U, 0x3f18417abdb2648cU, 0x3ea228f1bd373106U, 0x3ee2038ebf41c09dU,
  0x3eea8b76be522986U, 0xbdba5299be96bd00U, 0xbea74fe43f17d14aU, 0x3cb16ea23f489574U,
  0xbe1c825f3ecbc5ceU, 0x3ec5898bbea2dc2fU, 0x3d6661eabea3ea73U, 0xbec365b83e8c62c5U,
  0x3f8f45c13d87e30fU, 0xbe5a57203e6a15d7U, 0x3e08c84b3f32c935U, 0x3e22af763f1b0728U,
  0x3f2d61f8bf0829e5U, 0xbeefaec6bf3abac2U, 0x3f504e413e21b94cU, 0xbd1db68d3ed463e4U,
  0xbe9947913f0ab33dU, 0xbdc50cab3e86e8f7U, 0x3e1d1d5cbe7b7875U, 0xbf3bda153edd16daU,
  0x3f3d7411bcdc1b19U, 0xbe675372be4def46U, 0x3d763df53e898544U, 0xbf3e46483e05b9e4U,
  0xbec592aa3f46b74bU, 0xbf78578bbebd45faU, 0xbf27eb9c3f2aa12fU, 0x3f7afb7d3e968de4U,
  0x3f057a1ebf195c85U, 0xbedbc0b43f0f318bU, 0xbf38a7de3e92e76eU, 0x3fa4d9f23ef46d00U,
  0x3e28a09d3f144cd3U, 0xbf0c7a4ebe0b8d97U, 0x3d6bd2ac3ddf7d85U, 0xbf2e16213f24a8a4U,
  0x3f2bf0b03d8c331fU, 0x3ef2692bbf88534aU, 0xbf00517c3f19401cU, 0x3d0fdfc93eb2a293U,
  0x3f97f6fa3fb1fbd0U, 0x3f2ab8bfbd736aa3U, 0xbe2bf5eabf284693U, 0x3f081c343c9c37c7U,
  0x3f26fb8e3db34ed6U, 0xbef506383ea2fc16U, 0x3ed05351bddd094aU, 0x3e1997713db8db59U,
  0xbd7f8a4c3f467380U, 0xbecea87b3eef95ecU, 0xbf82a2013f6bb107U, 0x3ef9fa913e99fd27U,
  0x3eb5fbd23fdd46f4U, 0xbd88a18f3dfb3986U, 0xbe4e0432beb06b9dU, 0x3e3ee40a3f8e3025U,
  0x3f9680b43ed2dec1U, 0xbe121d4b3ea369bcU, 0x3d0094463cf73f9fU, 0xbedbbde23fae13f2U,
  0xbf01331a3f1e4a19U, 0xbeff9107bec590e2U, 0xbf1e880a3f21ab08U, 0x3f8bee243f02e9f1U,
  0x3fbebce93ead09a3U, 0x3e811d883ee2dff3U, 0xbf55112e3f02c795U, 0x3f1441ba3f1679bdU,
  0x3f03e8aa3eaafbbcU, 0xbede93edbeb54d43U, 0xbf2ac16ebe7ef35cU, 0xbf3571433f11c20dU,
  0x3eefce6bbec61b5aU, 0x3f199252bef3b357U, 0xbf5bcf233dee8c3cU, 0x3f2ef4483edb92c9U,
  0x3ebb1daebf291092U, 0x3f4cdf41bf0ba424U, 0xbed47d903e1b1f6dU, 0xbe5202de3e4116b8U,
  0x3eaed98d3d01bc31U, 0xbef1f8953e855923U, 0x3dd58acd3ef51834U, 0x3e375f983d3a3efdU,
  0x3e45db233e93f463U, 0xbdd0ca923e2efe95U, 0xbf07ccf240091481U, 0x3ec73e4dbe928e5fU,
  0x3e013ce63ef2877fU, 0x3ee0eab63cd97773U, 0x3f8c128ebf52eca2U, 0xbed460753f5ed1bfU,
  0x3fac667c3ded841fU, 0x3ed083d53f2c1d81U, 0x3dba4e963f7a844aU, 0xbf0006d33f65315eU,
  0xbd205f673f46b6acU, 0xbf31ccb43ea2924eU, 0xbd8b6d663f717c50U, 0x3f53efc13f1884b9U,
  0x3fced87ebf521866U, 0xbecde4093e02957fU, 0xbea8e61b3ebcafa3U, 0x3edda9af3e5cc8c6U,
  0xbda432a33ebfeea7U, 0xbee0c25cbdeaaafdU, 0xbf183d0a3e9d4a6dU, 0xbf02e9dd3f37d01aU,
  0xbe524a44be05dcc8U, 0x3e5d2742bedbb37eU, 0xbea1943fbd83ad32U, 0x3ee20230bebd38a6U,
  0xbe96b6183e200e56U, 0x3ce53df83e36c14eU, 0xbe213a01be4fe2c0U, 0x3f07b7a9be95606eU,
  0xbe8f8da73d5e1904U, 0xbe24fa2cbe6176b4U, 0xbe3197063d9f7768U, 0xbd4de2d23e47afc4U,
  0xbce8791cbe89ca71U, 0xbd10cfe6be384ee5U, 0xbe924846bf348f65U, 0x3e583e38be2f32daU,
  0xbdebd1a4bdfc06edU, 0xbcae59a83db38ba0U, 0x3e3cdcf8bea6802bU, 0x3f158c03be5441deU,
  0xbeb19b773e1ad159U, 0xbea9be8ebde0c036U, 0xbe69ed43bde87e61U, 0x3db8c4f13db41a67U,
  0x3de37bfcbed40232U, 0x3e85e06c3cbd93a9U, 0xbd218bac3e7390c5U, 0x3e748e59be56c02dU,
  0x3eb7da7e3eccbb5eU, 0x3daba8d8bea7f30dU, 0x3f027ffebea1dff6U, 0xbe892272be769667U,
  0xbccb1fec3c9e69abU, 0x3dc13f3cbe512cb5U, 0x3ddaa764bd39dc0eU, 0x3e3e6603bef27873U,
  0x3c14b44e3f09d0f7U, 0x3dedc0c83eb6acd9U, 0x3dd1da4ebf00d1eaU, 0xbda053923e366df2U,
  0xbeabc5a8be031303U, 0xbec26d963d424595U, 0xbe16e8b43e4e0712U, 0xbf45f63f3efccccfU,
  0xbe6b99333e14f596U, 0x3cdc930fbdf0ba9cU, 0xbe0688db3de5186bU, 0x3e54bc85bda51bbaU,
  0x3eb3f8a13e522039U, 0x3cb5ed023e4c91b5U, 0x3ed4a5c5bf6f4dd4U, 0xbf0a33e3be628074U,
  0x3f29cd603eac39dbU, 0x3cda8f183d732904U, 0x3f10e29fbed9d22fU, 0xbf0688283aca3ad4U,
  0xbe42f41c3e31902bU, 0xbc74ff17be4e9ab1U, 0xbd1409ddbe45f150U, 0xbee610b53dccb0dbU,
  0xbc6f85a8bdd7b127U, 0x3d2872b23d8d6c80U, 0x3ea6eb3f3eb4f797U, 0xbe03e5babec70c9dU,
  0x3e907a833dda87c5U, 0x3c6d887dbe823635U, 0x3e245196bd0740beU, 0xbf7d777dbeb7f33cU,
  0xbda5a94f3c6d4abbU, 0xbccf1ff5be519237U, 0x3ca1d4ca3e94b99eU, 0x3ebfdb57be817dacU,
  0xbe91bcd53e9947f9U, 0x3d8c82d9be3b2b36U, 0x3ec2c436be9dabfbU, 0xbe871b61bde41fecU,
  0x3f23e8fa3e891fe6U, 0x3ebf7a273dd9f2eaU, 0xbddec1debe924372U, 0xbe58dae9bef571c8U,
  0x3c92cd20be3139c0U, 0xbdbced9cbe8ea368U, 0xbe4a3733bc95b040U, 0xbcc7bed93ee01891U,
  0x3e7b427ebe8d111cU, 0x3d9191a93d920911U, 0xbe6aa7dabf2e80f2U, 0xbcd864dfbc201ec8U,
  0xbe819a9fbf14337eU, 0xbe309cb43ddaa186U, 0x3e2968873e6f8604U, 0x3f580121be85a496U,
  0xbec1c366bdc7883bU, 0x3c33b61fbe84a482U, 0xbe1059ab3f0e3550U, 0xbe8888003dca14f9U,
  0x3ebfbebebddc2039U, 0xbcb35b343d863e6aU, 0x3de2b0dabf30d07bU, 0xbf12269e3d8f0f02U,
  0xbf8cff9f3e2a284aU, 0xbd9bc976be40b373U, 0xbe36744e3d7b68f3U, 0xbf8614c7bd00858dU,
  0xbcf8e55f3c802d93U, 0xbbd77f27bea9e765U, 0xbe5cf3a53ef9bd66U, 0x3e34902dbec46aa0U,
  0x3d4c359d3f86bd65U, 0xbd4514be3e824d6fU, 0xbf466de9bf20c1a2U, 0xbdac983bbc80b524U,
  0x3f0c3ae03e94e9aaU, 0xbde81db03e6ba613U, 0x3f004ae73da81771U, 0x3f155a053da6e558U,
  0x3d9c8b40bc47f2a4U, 0x3ac373a33d5e9e6cU, 0x3dcba35dbeedce3aU, 0xbe0a3c4abea267d7U,
  0xbea9d08a3ec595bdU, 0xbe67c9b03d8dcd9bU, 0xbeeed0083efb0016U, 0x3e28aa2fbe8d3472U,
  0x3f65fa8ebeda691fU, 0x3d55b6733e0fe3deU, 0xbe61441ebd4f20baU, 0xbe6b240a3e546f06U,
  0x3e7195b2bdaaaadfU, 0x3ed311013ea7d42aU, 0x3dbb4af83dc7b0e3U, 0xbe4081003ea24317U,
  0x3d782f4ebf9df0caU, 0xbdb077e33d01176dU, 0xbeae7797bf116423U, 0xbcde0354bc6219e4U,
  0xbec19b71bf0088a9U, 0x3e11c3bb3e03fd6eU, 0x3b63518b3c24851aU, 0xbf9e631cbde1e79aU,
  0x3e85d20c3e12252eU, 0xbdecede73e0de4bfU, 0x3ebbefe03d19a042U, 0xbd960f593e14730bU,
  0x3ebb8db73f397c3eU, 0xbbf5f0383db30895U, 0xbefa2a9cbf030904U, 0xbf3089ac3e58e0fcU,
  0xbf4f7615bcb17457U, 0xbe80031d3c803446U, 0xbe6c19443e1d02c5U, 0x3f03102b3e0e3f81U,
  0x3d357d493e8278d2U, 0xbe1a3510be972181U, 0xbecb091ebcce836aU, 0xbcdd225ebebcad81U,
  0x3e4e9869be305b4bU, 0xbeb0c704bee3e4e8U, 0x3dd6122ebf296a5eU, 0xbe57925cbe8b34eeU,
  0x3dc4fa26bef0796fU, 0x3d3419d53d52f558U, 0x3eaee0033e68eaa2U, 0x3ebd828cbd04f00eU,
  0x3e706272be4b2919U, 0x3d7d83c63e92b8aaU, 0x3d7442d93eac7d4cU, 0xbe56962cbb88eec4U,
  0xbe4441733f2acb84U, 0xbddba6fabec630abU, 0xbec8c8cb3e83b5b1U, 0xbdff6117bd6a31e9U,
  0xbf87aca7bf373988U, 0x3edec71e3dedf5daU, 0xbba82ac2be8dd2e2U, 0xbe5e9652bea75e38U,
  0x3e8997513e86e018U, 0x3db9d3eb3e9bb089U, 0xbd8f7970bdad7076U, 0xbe4232cf3e423644U,
  0x3cd30e3f3e0192c0U, 0xbeb018a0bf34e27bU, 0x3c655dea3e1241deU, 0x3d49d4753d7ee8cbU,
  0x3f30534d3e97b223U, 0x3e88af503ed44e9aU, 0xbf050008befdaf3bU, 0x3f4a57b93f0b8467U,
  0x3d89af843d4d5e3fU, 0x3d965a0e3ea04057U, 0x3e245e513e001f1fU, 0xbe89724ebd25a5bcU,
  0x3d7be3e63fad8173U, 0xbc5447bc3e471e79U, 0xbca39e3fbf0eacb2U, 0xbd6a5f863e4a1444U,
  0x3f1b3aa53ed3815eU, 0x3e56461c3e38847fU, 0x3da203eb3d906701U, 0x3d06bb7cbea14bc6U,
  0x3df9f54f3dc2f934U, 0xbe5e18aebde239eaU, 0xbe1cee033d37dc09U, 0xbc8d6152bf1872ffU,
  0xbe9e093bbf37ceb8U, 0xbdcd194fbeb178e7U, 0xbeab976bbdb1e0ffU, 0x3efbf390bd8f3e73U,
  0xbe9e0106bf328259U, 0xbe25381a3e9a28ceU, 0xbdebe8b6bcf37de4U, 0x3eefa029bca5a57eU,
  0x3ed4e3843eb5e528U, 0xbe5222613ebc84f5U, 0x3e677346bd57b94eU, 0xbe2d20cd3d1ad118U,
  0x3ebfdd74bf84d28aU, 0x3d07cc8dbe8c38dcU, 0xbe2e50073f16afd1U, 0x3e97d3c63f58529aU,
  0xbe4792eebe191824U, 0x3ebcd680be39300dU, 0xbedd9b18befa8588U, 0xbea571d93f099675U,
  0x3f2073283dc07121U, 0xbe952e8b3e4967b0U, 0x3dfef4eb3e2331faU, 0x3d84b2a23f14f58bU,
  0x3e8c6185bd2935afU, 0xbeaec765befcd0abU, 0x3e191be53e271afaU, 0x3d620c073e44de47U,
  0x3ed8e1993d4a7cdaU, 0xbdad7b253d98a20eU, 0xbd71a877bf005223U, 0xbefe28763f009a41U,
  0x3f61f116be93d61dU, 0x3d1123e83dca8443U, 0x3e830dfd3e6c0a61U, 0xbeefb5ff3b6b7db1U,
  0xbe76aba23f5c5b86U, 0xbf2374f2bef3d031U, 0xbf225c6e3f0f3da1U, 0x3ef6ade83e053a59U,
  0x3f516223beaa507dU, 0xbef1e01e3efb8593U, 0xbf2f114ebcf43560U, 0x3f9e254d3ec28b76U,
  0x3e44e3d63ece83aeU, 0xbecd35f83d4d1797U, 0x3c91a88c3d50578aU, 0xbef683e13ee314c3U,
  0x3f067103be6ede48U, 0x3eb4f460bf1edd84U, 0xbe9fdef53f10c815U, 0x3ec618263e9e8548U,
  0x3fc6d6673fa957bcU, 0x3f357c61bdd020a1U, 0xbef2f642bf2a8f4dU, 0x3f360be2beb52a5dU,
  0x3ee80339be2af9fcU, 0xbec2b12c3ea29b35U, 0x3e58bb8d3e599ad4U, 0x3e5cc7e53ec9b603U,
  0xbd9d715e3e9ac514U, 0xbee7df243e079ef9U, 0xbf0fd0143edb9389U, 0x3e9318babd36f9caU,
  0xbe1f1b6f3f4f41c1U, 0xbd7f70663eb3e953U, 0x3e99dfb23dd32116U, 0x3f1d6c663f6856f6U,
  0x3f9b96e93eb327dfU, 0xbaf49e7e3ee89d19U, 0x3e2dc73c3e82478fU, 0xbe8c5bf13f6b8f52U,
  0xbf02272c3f8d7ab9U, 0xbef06e85be9ea36eU, 0xbf36d9a53f5751a5U, 0x3f4d11dc3f0d6934U,
  0x3ec8ed4d3ee42994U, 0xbe3807ea3ed34c69U, 0xbf441da33f079765U, 0x3ea40ba13f1425c3U,
  0x3e8fc1de3eaa9427U, 0xbe65429cbe233e9cU, 0xbed45e9cbdd01b81U, 0xbf232a303efdd630U,
  0x3efa1830bf81cc61U, 0x3e52ed81bec2dd1aU, 0xbf3f88943c0ab54eU, 0x3e4383703eb6c398U,
  0x3f2e1904bec39d3eU, 0x3f4bbf0ebe87c32aU, 0xbe9503573e464dacU, 0x3d5fca523ec5a23aU,
  0x3f5a4b183e2f8d5eU, 0xbd57574e3e90b1b7U, 0xbbb433d83e9eb17cU, 0x3df3ebc53cbf6510U,
  0x3da24519be9092e0U, 0xbe7c02e03da7fd95U, 0xbebfd0573faa1dd7U, 0x3f0f611cbf04a6d8U,
  0x3e98df4d3dea6004U, 0x3eebb14e3e3027a0U, 0x3eba89bbbecbe12bU, 0xbe34dbc63f275f1fU,
  0x3f635e25bd971e0eU, 0x3ed704563f05de8bU, 0x3e9340bf3f0e482fU, 0xbefb66343f448e62U,
  0xbe3215823f388708U, 0xbf223dee3d635d9dU, 0xbe19b7f93f98ef59U, 0x3f135fba3f1f122eU,
  0x3fa52b6fbf39e7beU, 0xbd8061ef3eae10e5U, 0xbe91f6633ea4022fU, 0x3e660f2b3e73a3d6U,
  0x3e8df92f3ea7aa49U, 0xbe9286c73d3a6678U, 0xbe2f0076bd0317f9U, 0xbee4958c3f322e95U,
  0x3e6d99643e580deeU, 0xbe3490d5bea49f33U, 0xbf2a0db5bf78c9f7U, 0xbe1b0c0f3f28ad9aU,
  0xbdfa78603de63ffdU, 0x3e2143e83e5ae798U, 0xbf36311d3e0ef966U, 0xbf384d75be418774U,
  0xbde030273ebe4d3aU, 0xbf2c7ecdbe5cb968U, 0xbe06ccf73ca31feaU, 0x3c880cdc3d8b336cU,
  0x3d6a55d6bf064e99U, 0xbe91f9f7beb59f5fU, 0x3f372a79be0b1561U, 0xbe83286c3f0999a0U,
  0xbf2acfb6bea72572U, 0x3edec8a73de6d3d2U, 0xbf68a6e4bf785c18U, 0x3ee87f88bdf49875U,
  0x3ee31b2fbdb6f6a6U, 0x3e1ff6153e3616b8U, 0x3d9f97afbdae0063U, 0x3d91d27abeaf016dU,
  0xbeb8fda2be4b3f7cU, 0xbf00685cbf172becU, 0xbf6fe568bf94ef52U, 0x3f38a9f7beb17ed4U,
  0x3e40670cbf61ce41U, 0xbf0be7013e9bbf8eU, 0xbef37805bec1a212U, 0xbf7bc2ad3dddad83U,
  0x3f6542473f294158U, 0xbf3dcd2b3de01ff8U, 0xbe4ea46ebc8beb26U, 0xbecf3dc33f4ec5d6U,
  0x3e65ff883e34b406U, 0x3d7b40d1bed5aa15U, 0xbecb996ebede9888U, 0xbb64bc083f515fa5U,
  0x3f10645bbf84918dU, 0x3f03fe943ecdf33bU, 0xbf42f04cbe8e6715U, 0xbf028e31bf4f721bU,
  0x3e946c4a3e91476bU, 0xbdfbc384becefba2U, 0xbef266c43e598b20U, 0x3e53344a3dd1c8d5U,
  0xbc98af373f586221U, 0xbe92c86ebe790dfbU, 0x3f3ca1a7bd8cad1dU, 0xbe82eed83f092335U,
  0xbeb54f6fbf1d8d13U, 0x3ef1c75d3e6e500bU, 0xbe807296bf18034eU, 0x3ec2f9f0bf3334a3U,
  0xbf105fb73cda2745U, 0x3e9cf5aabc15037bU, 0xbd8626dbbd93227aU, 0xbdeb6f0e3eaea394U,
  0xbe83a2edbe5db37aU, 0xbec9a78cbf4bcccaU, 0xbf38dc8bbf3e96aaU, 0x3ee9f297be701576U,
  0xbf13e052bf45d79fU, 0xbf367f143f0c606bU, 0xbf529450becac953U, 0xbfa152d9bed1b4e3U,
  0x3faefd853f1cb4beU, 0xbeecd5dd3d49cae5U, 0xbe8cea05be6af664U, 0xbf15b5e63f6b1748U,
  0x3e1af4ca3f366004U, 0xbe611731bf0eebf8U, 0xbe3dda9dbf54e402U, 0xbe6f19743f624fe8U,
  0xbe7c918fbdb0d5d5U, 0x3e99a2133e89bae6U, 0xbe3e77ddbf24dd56U, 0xbecfb97cbf6191efU,
  0xbcecc3463ed5c253U, 0xbef36fb1beae6ff9U, 0xbdd482e73da721a4U, 0x3e9d8154bc8afca8U,
  0xbd187ccc3e957f3eU, 0xbecc3318bf13d4cdU, 0x3f23432fbd99e923U, 0xbd56f9803ec459f9U,
  0xbc5815a3be4993fbU, 0x3efceeed3e781e2eU, 0xbe24688abe7b948cU, 0xbe47da4bbf5e31c1U,
  0x3f1af3e13e8d50bfU, 0xbdd674c73dd493a6U, 0x3d813c403e297ea3U, 0xbd4e764e3ca5dcc9U,
  0xbe1c4fdf3df3604fU, 0xbee55f9cbf8e2579U, 0xbe086829bf990465U, 0x3f50d212be55a960U,
  0x3e98c700bf834623U, 0xbf7442de3f105babU, 0xbf144b83bf12eb31U, 0x3f5d1865bf809e99U,
  0x3f8f833a3f552606U, 0xbf4b8cdb3e1bb29fU, 0xbdecda683e49b885U, 0xbee31c2b3f73d0c7U,
  0x3d7b8cdebeb1db19U, 0xbe2a60523ea58025U, 0xbbefdda53e6e0630U, 0x3f07bd88bc2caa61U,
  0x3dfae229be624684U, 0xbf09fc593db8a237U, 0x3eb0a4533ec1e55eU, 0x3edaebb2bf008bbaU,
  0x3edcb45b3e000c8eU, 0x3e68c2093e31d2d0U, 0x3eb955b1babb8dbaU, 0x3e09b9083e9b36b4U,
  0x3ec9ef2bbeffb48cU, 0xbded83153c954f1cU, 0x3df33cb13c5f2b23U, 0x3f1419123da0eef1U,
  0x3eab963a3e52688dU, 0xbc6c45f7bdf2bbc9U, 0x3f05da8fbe3ced7fU, 0xbe951f523e560f1eU,
  0x3e9f9bdcbe238803U, 0x3ee9be3c3e813fe0U, 0x3eba53c8bebecf7aU, 0x3db836f63d07e54dU,
  0xbe1864413ea7c0b6U, 0xbe535f12bde2e9f1U, 0xbe35bb583dd34063U, 0xbeadba72bdc4059dU,
  0x3f515c38bc228117U, 0xbd2738c13e0bdb09U, 0xbea0fbb83e3e0731U, 0x3f1c35ee3ccf312fU,
  0x3d70ef5dbd1d4383U, 0xbd24bd6c3d33a634U, 0x3d7f0818bf099276U, 0xbc044ede3de5bf12U,
  0x3e4e8a72bf302a70U, 0xbe0ee2293e680b13U, 0xbecc3fa4bf067b7cU, 0x3e57be043e6ca921U,
  0x3e1ae6683f8bc812U, 0xbdb957c4bd9f4f58U, 0xbf2c8155bdda42f7U, 0x3e4e21a9bdf2120bU,
  0x3ef88f4d3d057a4dU, 0xbcde79bb3e83b966U, 0xbe4a6c9dbea386f6U, 0xbdb76fd9be6dd124U,
  0xbdfe9caf3f4ac78fU, 0xbe1be6e33e8add0dU, 0xbd9b94e03e8ada74U, 0x3e0662b33e6d21a3U,
  0xbf22ffa43e95fb9cU, 0xbdf0f5d23d090b8dU, 0x3ec5ce363df7b997U, 0xbf0da7513e46abe7U,
  0x3e63b10dbdfac7a2U, 0xbe30777a3e00b152U, 0x3e79060a3e6be565U, 0x3dfb926ebdbd8d56U,
  0xbe53e1e33f468cc4U, 0xbe55eb633dcb6034U, 0xbe95e9a43ef808a1U, 0xbd17702a3ef5a97eU,
  0xbef9930e3ec6e46eU, 0xbc67285b3e338bafU, 0xbef11f653e64a5ceU, 0xbe98ffef3d3f1c44U,
  0x3dd05d3a3e3152ffU, 0x3ddf52e7be2fe502U, 0xbcf0701fbf04bc7bU, 0x3d681f513eb55e35U,
  0x3e1f9ce43f2288d9U, 0x3d96fa9bbe8c371cU, 0x3e8cbc46bea49a63U, 0x3e1791983c182cefU,
  0x3f6afb943ec2a23fU, 0x3ef98540bda8e070U, 0xbebb9f0a3e8bafccU, 0xbf059caabcd7db7fU,
  0x3e8e587d3d4e082aU, 0xbd102be0bcc1b1d2U, 0x3d2390bc3dbdbf8dU, 0x3df754373d82fc90U,
  0xbe12979cbefc5870U, 0xbc7eca1cbe57b181U, 0xbeeac5d13fe8d007U, 0xbdce6d6abdfafd11U,
  0x3ef5f7813e8ce647U, 0xbdc1e5783d0c6c20U, 0xbeaea69b3f1264deU, 0xbf1a708bbe1a3e99U,
  0x3e2eac3dbd5cc6acU, 0x3dd108c03ec55054U, 0x3e5d5f7d3a865c43U, 0xbe888accbeaae913U,
  0x3d8110263f6081a7U, 0xbf0b34893e6c8262U, 0xbe94fca9bdead3f8U, 0xbe8bd4743f0ad282U,
  0x3c855d58bf0c0eb6U, 0x3eb86c3f3c0f9122U, 0xbe3672043a23a3c8U, 0x3f0097313eafa708U,
  0xbe3b55f53c55708cU, 0x3def746e3ce64858U, 0x3e738140beac9b1cU, 0x3d3a81a83d10acc7U,
  0x3e575435be245d77U, 0xbc989620bed1e4b1U, 0xbf0e8c34bf3aac8eU, 0xbe8292da3efd7415U,
  0xbeeb2b7a3e30eb83U, 0x3e1b76423e9fdbcdU, 0xbeb254e33e80902bU, 0xbe33cde8bf05f85aU,
  0xbebee9793e966b07U, 0xbed5a3d0be346705U, 0xbe34d2afbeda85c0U, 0x3e2f50acbe64bd00U,
  0x3d684576bd1098d2U, 0xbe96e7abbefac0ebU, 0x3f28fecd3e515c9cU, 0xbefa0b133ee51404U,
  0xbf2d451cbeecf52cU, 0x3ded320d3e440855U, 0xbf11f5cdbf3f5e88U, 0xbe4a6aa9bb58b290U,
  0x3ecf7197bd836a84U, 0x3f0488193e5a7b06U, 0x3dab21573d93efd5U, 0xb7bfdbf0be3360b2U,
  0xbe8ac5b83d93f755U, 0xbec8fa8fbf2abddfU, 0xbf35e43cbf934e91U, 0x3eaf5974be9d289dU,
  0x3f0aa328bf174f92U, 0xbea75f7d3e85054eU, 0xbecec3ebbe9e9aaaU, 0xbf9799b63d169114U,
  0x3f247e503f04c92bU, 0xbf17df1f3e5c1e59U, 0xbe0f90053e5a5ba6U, 0xbec6031a3f328401U,
  0x3e1869e23ed13b04U, 0x3b528f35beb220ccU, 0xbf136783bf922832U, 0x3bbc3d273f5a0855U,
  0x3f4a811abf6267cfU, 0x3ebea01c3e8d0f54U, 0xbf146dffbe8f7fdcU, 0xbf37dd05bf17093bU,
  0xbc7fe4d63e9b5581U, 0xbe9f617fbe9f0c44U, 0xbf12bcb8bd1802deU, 0x3c6af2fbbb11894fU,
  0x3e58364a3f410ceeU, 0xbf03945fbef3928dU, 0x3f704f43beadcb37U, 0xbea6d70e3ed2714cU,
  0xbea98f22bf3d2351U, 0x3ee269733e844a14U, 0xbf10a3a2be89546aU, 0x3f5657b8bf3737b6U,
  0xbecc06a7be3dc567U, 0x3ea22dd33d6439e1U, 0xbd7138813bb741aaU, 0xbe040b2b3e34581cU,
  0xbdd8ce063d82cd5bU, 0xbed9c725bf509fa4U, 0xbf809355bf360df3U, 0x3e93af79be01c313U,
  0xbefa733fbf10ee52U, 0xbf00fd223ede2b10U, 0xbf030960bea97398U, 0xbf8ff011bec90735U,
  0x3f720f9f3f311195U, 0xbef173c13dcf15feU, 0xbe14b51ebe6b10c6U, 0xbed391193f4123e4U,
  0x3deb2b3c3f436621U, 0xbcb16c52bef5a8eaU, 0xbecaecd3bf68ae00U, 0xbe46ace23f261509U,
  0x3f3d83db3e31781fU, 0x3d8ad2373ece9555U, 0x3e784fc0bf5f5251U, 0xbf7692fcbf462fb8U,
  0xbe3b8a923ea2688cU, 0xbeb652edbe86557aU, 0xbd183fc0bd955c71U, 0x3e71ca2d3e912badU,
  0x3d0b09ef3ea0eb09U, 0xbead46f0bf0d3213U, 0x3f2538fcbee15625U, 0xbec606ac3f1addddU,
  0xbeb3d487bc9e9d74U, 0x3eafcb033eeca19eU, 0xbc8f2351beb116f2U, 0xbf067a67bf0e9a98U,
  0x3e1f7ead3e435628U, 0xbe3086fb3e16f8adU, 0xbd06b350bd85dd10U, 0xbda78b003d14d467U,
  0xbe78fae6be9fd187U, 0xbec94955bf4fdc49U, 0xbea9d4c5bf78cd26U, 0x3f1b8b5dbe287638U,
  0xbd202ea5bf591dc4U, 0xbf67e6d73f048a51U, 0xbe3e5efcbeb373eaU, 0x3f2f19fbbf3fef34U,
  0x3f4275573f3c999bU, 0xbf06ba1e3e5ebeabU, 0xbd9520e2bd83f6aaU, 0xbeec49a23f49a4b9U,
  0x3e3c49b7be572819U, 0xbeaf0f6abc4628acU, 0xbe564b253f6ddb1fU, 0x3f0c90ce3f0e1313U,
  0xbf4a80a2bf48f27bU, 0x3e4360f53e0cfb2bU, 0x3f298de1bdabe368U, 0xbec6cbe03f0f7a4dU,
  0x3f087034bd52b554U, 0x3e95cdfe3c6f0551U, 0x3e12ed0b3dc6b73bU, 0xbd4d8befbd99693dU,
  0x3d02469abf0bfbc2U, 0xbe2f69653c99ded7U, 0x3ee14c443e097bcbU, 0xbe7169ca3ecb7fcbU,
  0xbe79a3adbe7827c0U, 0x3f18e605be743103U, 0x3f23bff23db77e8fU, 0xbfb4862c3cb35b69U,
  0x3ef970cabeab9c17U, 0x3e8fd1a63e4c1a5fU, 0x3eb141c83e3870a1U, 0x3e4eadd63e95c298U,
  0xbdb88d6a3f194304U, 0xbe2f5d373eae3e83U, 0x3d2c38adbe5a6fe8U, 0xbea184eb3da8411eU,
  0xbb45997c3e67e420U, 0xbe123dd53e7229ffU, 0xbe26b0163ea8e1cfU, 0x3f2431973f2dd7b6U,
  0x3e971f07be13d49aU, 0xbdf4d5f03e54171bU, 0x3ee415b1bd2ad2beU, 0x3d8fd8593dd9f0ebU,
  0x3ea2ad5c3da89c6fU, 0xbee5ce9a3dd9edbdU, 0x3d994cde3f2a7de8U, 0x3e510a323eb7990cU,
  0x3f5d559a3ecaa7acU, 0xbe6f2bfa3de0b3ccU, 0xbe864a0fbda8af87U, 0x3e1e00ebbcd36748U,
  0x3efb7f923e532583U, 0x3e153c813c08ad7dU, 0xbdc4ccda3c2ceadeU, 0xbe364cc6bd9df8efU,
  0xbd83f90c3e5c8694U, 0xbebe7d95beb78508U, 0x3e83782b3ee25bbfU, 0xbd13f5603de3ef8dU,
  0xbe49fd3f3f50c8c2U, 0x3d8efaf03dd468e7U, 0xbe102bd73def24ecU, 0x3db3f0eb3e1ba47dU,
  0x3efcadbfbe11cc60U, 0x3d10127b3e6f4ac8U, 0x3e3a88e83eac27c2U, 0xbe4557353da00868U,
  0xbe1bc6bc3ebea135U, 0xbe709c113e67aefeU, 0xbe6ea9eb3e3aada1U, 0x3ddca14c3ed45d2bU,
  0xbf41c7ae3f179e24U, 0xbe61d6193ea9953aU, 0xbf0c2629be86c284U, 0x3f4f50ba3f0055c9U,
  0x3be35bc23db49a5aU, 0xbd8c4cd83f2d2c2eU, 0x3dc72ddf3effce45U, 0xbd91f1c7bc720ca3U,
  0x3f0c1e10bf1160b4U, 0xbe301116be9054f6U, 0x3d6d6d923ee448e0U, 0x3f0c40073e46040fU,
  0x3dbc9534bf071aeaU, 0x3ce52402be1804c2U, 0x3f2eb4e33e0a5e5fU, 0x3f5d19e83f0b4007U,
  0x3ed3b661bb2855b9U, 0xbe3fa8633e447588U, 0x3ebc3df23dfcaf30U, 0x3b995907bd76ab92U,
  0x3b6c3cff3eff5349U, 0xbe5a15cbbe0c3d79U, 0xbea7aa9b3f688a26U, 0xbecc0b6e3e6b9438U,
  0x3f35bd0f3ed905caU, 0xbe6a03bd3e12d6d6U, 0xbcc3be2b3ec4cb05U, 0xbd861b723ec0da58U,
  0x3ea738593f009ff9U, 0xbebc34fe3ea12a01U, 0x3ef8cdd7bc1fcfb5U, 0x3da683483eeda1b2U,
  0xbf205752bee5d592U, 0xbe5b052d3e43bd24U, 0xbe00bac13ecc11ffU, 0xbcce9af73f13d658U,
  0x3efdf05d3d6a47d3U, 0xbdc3763f3f059309U, 0xbe23b88a3ec0b7dbU, 0x3ea7cfe53f080d07U,
  0xbe8384ea3e803da1U, 0xbe48e1e43e56d735U, 0x3ee7de5b3f166b0fU, 0x3da54ca13ed58eeeU,
  0x3e8914653d3c9c5dU, 0xbdadf5163e53b5bdU, 0x3e0aa94ebe8fb488U, 0x3e0f0c45be72383fU,
  0x3f0adf4d3e9c0836U, 0xbe1a9ebcbdb82de3U, 0xbe02c33dbee73719U, 0xbf56b33ebea243f0U,
  0xbdb1f2393e8e1a34U, 0xbb4a64f8bdb2ba91U, 0x3cf6ce56beb7d574U, 0xbd6c48aabee71cd4U,
  0xbe19fa233f2d870fU, 0xbdcade0bbe3263fbU, 0xbf49e8633d30e571U, 0x3e4f15513f1222b1U,
  0xbe7bd2ce3dac5009U, 0xbea41653bc2898fdU, 0xbf336771bee93cb1U, 0xbe476aa9be1029acU,
  0x3ee2fa903c5d6662U, 0x3c1eadc03e083bddU, 0x3e4ac9333bda0483U, 0xbbb2f9043c1cc2dbU,
  0x3e893400bf45ce17U, 0x3d98ba66be938137U, 0xbe465a3abead9bf5U, 0xbe3b10d7be1620efU,
  0x3e80a578bea01aa9U, 0xbd8c88073d31b8b7U, 0xbef5cc30bd910642U, 0xbfd5f635bdb697c0U,
  0x3ec7fbe9bd4dc1c9U, 0xbe4ccb0f3e8836caU, 0x3e2b8b213dd688e1U, 0xbdd6de8fbdc3bae3U,
  0xbebf4f46bf16133cU, 0xbe99e7a1bf01d2beU, 0x3d55a3f5beba4b74U, 0x3e7459443e6217abU,
  0x3e878ff5bea159ebU, 0xbeaa889c3db59a1eU, 0x3e164a7abe702d8fU, 0xbeb18749be8ca83cU,
  0x3def71d93da5192bU, 0xbe14b35bbe932be4U, 0xbd02b80abe9f291aU, 0xbcb21939bc75f335U,
  0x3d0dffc23d2b828fU, 0xbe5db91f3e9fd9c8U, 0x3e32c62d3f0fb360U, 0xbf63efc93e64e6abU,
  0xbdc1fc83bf89cd07U, 0x3e681e563d786888U, 0x3f1c35093e1cb869U, 0x3f39e0a5bef22ebeU,
  0xbf0eccd8bd17ffbdU, 0x3dc8fe1abe1be22dU, 0xbe81be533e5a2445U, 0x3e46db3cbd027ec2U,
  0xb9bdea4cbf207522U, 0x3db02386be0dd005U, 0xbcc741dd3d1f16e9U, 0xbe1a758dbe8fe9f9U,
  0xbe4508afbe9bac7fU, 0xbe211eb23dc0639fU, 0x3dca5828befe59f5U, 0xbf2a6881bed130b9U,
  0x3e92b5423ea2305cU, 0xbd7b1f243ef9d768U, 0xbe912df83d6f0b4bU, 0xbe4263583e6cf153U,
  0xbe0f42d2beff1702U, 0xbde1f381be407173U, 0x3e2a53e73e97f1dbU, 0xbd2be4763effbcb2U,
  0x3f583afe3e8d5d81U, 0xbe6725e33eb57dddU, 0xbe93e759bdb1e83aU, 0x3e7ff2acbe1ccd43U,
  0xbe9aff253e8ccdf0U, 0xbdbff4553d0b17ffU, 0x3e0b6e2cbe03fd91U, 0xbe8595eabede9f5fU,
  0xbec762583e63a522U, 0xbe167c5fbe50f47fU, 0xbd852fd6bfa71652U, 0xbe510ab13e38e962U,
  0x3f37c705be355147U, 0x3debf3c63ed5f7b2U, 0x3eb438bebde0eba9U, 0x3f5201083e8baa04U,
  0x3d1d79be3df0f5f1U, 0xbd8831c9be993ad5U, 0x3e553481bed5df94U, 0xbd0bb6b8bd26c369U,
  0xbe2a4b8bbf069aedU, 0x3b615e7cbe79a38eU, 0x3eb11552be82e0f6U, 0xbdbc2f3bbf000c62U,
  0xbf448cb8be2495c2U, 0xbeabec6f3ee6c5ecU, 0xbdae0e3fbd7f6ea1U, 0x3eb99f1abeadd32fU,
  0xbc818e773e84563cU, 0x3d830b613db07f93U, 0x3de7ccc0bf08fcb8U, 0x3d3a2783bc163162U,
  0xbdb4bc0b3f0072c6U, 0x3d9a03ddbe19aab4U, 0xbe2a8d663e430b08U, 0x3e35a3e03eda78e8U,
  0xbb426673bdb10352U, 0x3eeeb8533e4ddf21U, 0xbe0c94c0beeb5ce7U, 0xbfa0ec6abea85881U,
  0xbd9dbea4bdab61d1U, 0xbd79d901bdc22839U, 0xbed5672ebd3b9550U, 0x3dcfa0843cc93d86U,
  0xbe4345fe3ea816b5U, 0xbe330ff3be49ea62U, 0xbefcfebc3eb843d9U, 0xbbf94fd53d92134eU,
  0xbf70b093bdd524f2U, 0x3d8c74273d499c61U, 0xbd315a0ebe40476cU, 0xbed275043dbe8afbU,
  0x3ecadbffbd8d7a3aU, 0x3ebadcf93e5cb5bfU, 0x3bcacdafbdb36ecfU, 0x3d59dababdce3667U,
  0xbda31722bf155d63U, 0x3e1ae2c1bde1de5aU, 0xbd5bc89ebd862278U, 0x3d8826aa3ccd3f9bU,
  0x3eb2e46e3dd160a8U, 0x3e981af33e0b8534U, 0x3e35653abea32856U, 0xbf9962dbbe4c2c68U,
  0x3d9355583d87e1e2U, 0xbe391d9c3de95b68U, 0xbe6d76b93e8d24c4U, 0xbdf78e23bd00b8d8U,
  0xbcc1717ebf09f74fU, 0x3ddb0759be8b266dU, 0x3d8b0d23be20f9c7U, 0x3dbea0043eaa3bc1U,
  0xbebbf887bf38ce04U, 0x3e94fa80bca66df2U, 0x3eb17d2cbece11d7U, 0x3ea6e74dbde2caaaU,
  0xbeb709d1be3214c4U, 0x3c1205e5bdafa466U, 0xbc18c4e9bd1d5dc8U, 0xbdc28226bd81b311U,
  0x3d9e63fcbe7ca0abU, 0xbdd11dc4bf5903faU, 0xbe166c713ebfc916U, 0x3eac685ebe68af8cU,
  0x3cd0bc7fbf1f3a38U, 0x3f300551bd8e5152U, 0x3e394d083e1f2971U, 0xbe4e4930be4cd720U,
  0xbdc408d2be83152bU, 0x3e7691fd3dbe3f8fU, 0xbc24327a3ec1f612U, 0xbeb6b0663f1e8043U,
  0x3dc84c1a3e30f92dU, 0x3e1ab460be3776ddU, 0x3ecf8aa8bee1dc2fU, 0x3d8ba647bbeae96aU,
  0xbe0437eabd5fa893U, 0xbe798e8dbd11a66bU, 0x3ed58a63be4cd35dU, 0xbf943a273e5857bdU,
  0x3e9567e1bd8ecbe4U, 0xbd1f429f3ec830d3U, 0x3da48e63be498a7dU, 0xbe216bf43e837965U,
  0xbabe7c493e6b4aa9U, 0xbd4f0e6dbead4c25U, 0x3d0eaa5b3e643b20U, 0x3e057a0ebe5f5437U,
  0x3e264d833f0dd066U, 0xbe567280bd79c730U, 0x3eae22cdbf23d6fcU, 0xbe9006983f4b7e10U,
  0x3e64a5d13b44fa34U, 0xbd0e83cfbe50f72bU, 0x3d4c0d52be1891f0U, 0x3dfd49dcbda285a3U,
  0x3e220b89bead0e15U, 0xbd1ff3f23d2586faU, 0x3d15a5f73e12798aU, 0x3e7fff1dbe6fe389U,
  0x3eb812f03dc00c4aU, 0xbbee236a3d9ecc0bU, 0x3f7445483e6c931dU, 0x3f1d4ea2bead175dU,
  0xbe8b70963d0af17cU, 0x3e01dc273ce0de8bU, 0x3de709233ed3596fU, 0xbe057577bea733a2U,
  0xbe880cc7bddbe068U, 0x3d9875abbe874cdaU, 0xbeb6de293cea4458U, 0xbdc26129bdd69223U,
  0xbe36dab4bd85c33bU, 0xbe6dd87e3e8cf7aeU, 0x3c8f1fecbe332386U, 0xbf0a97d8be39114aU,
  0x3ec1c9f43e89e1a7U, 0xbca542403d909131U, 0x3df334ff3e507049U, 0xbe15eac7bd7377a5U,
  0xbf0037f14038b0e9U, 0x40578678c08e41c6U, 0xc0bf48f9c048ce83U, 0xc0490a4ac00281c9U,
  0xc01f0d2ec0475da9U, 0xbe657ed4be6aa98aU, 0x3e8022fabca37be3U, 0x3dcf1b573e135fe8U,
  0x3ea8fa303d0de6bfU, 0xbdce99dcbeb35e48U, 0x3e11b714bea8114dU, 0x3e068f5dbe36918fU,
  0xbda43e883f94a70bU, 0x3ed08dfc3e3e17c9U, 0x3e06670cbe28e78dU, 0xbecf1c1bbe627136U,
  0xbfc7e61fbeb9deffU, 0x3cbd55e9be82ecfaU, 0x3e5e58f2bb5a0080U, 0x3dd56b863d949546U,
  0x3e83345b3d53e292U, 0xbe13b88fbea9e5cdU, 0x3dbb48d0be97a4ebU, 0x3e071930be0c19b7U,
  0x3dd88f01bc6c9abeU, 0x3dbe89cc3e1836ddU, 0x3ea6afb43d9c72a7U, 0xbde5ea46be1dcba6U,
  0x3df30816be20c7b8U, 0x3e227d9dbde4e847U, 0xbedb019dc01289f5U, 0x3f2b81bc3fb63967U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

