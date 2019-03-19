#include <algorithm>
#include <iostream>
#include <Windows.h>

#include "netxpto_20180418.h"
#include "huffman_decoder_20180621.h"

using namespace std;

void HuffmanDecoder::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}

bool HuffmanDecoder::runBlock(void) {
	bool alive{ false };

	/* Avaiable bits on input buffer */
	int ready = inputSignals[0]->ready();

	/* Avaiable space on output buffer */
	int space = outputSignals[0]->space();


	int saveOrder = getSourceOrder();
	int readyLimit;

	if (saveOrder == 2) {
		readyLimit = 3;
	}
	else if (saveOrder == 3) {
		readyLimit = 7;
	}
	else {
		readyLimit = 15;
	}


	/* Cycle to process data */
	while ((ready > 0) && (space >= saveOrder)) {
		int counter = 0;
		bool symbolDetected = false;

		while (symbolDetected == false) {

			symbolDetected = getData(counter);
			counter++;
		}

		/*
		int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;

		switch (sourceOrder) {
		case 2:
			inputSignals[0]->bufferGet(&a1);

			if (probabilityOfZero > 0.5) {
				if ((a1 == 0)) {
					int out0 = 0;
					int out1 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);

				}
				else {
					inputSignals[0]->bufferGet(&a2);
					if ((a1 == 1) && (a2 == 0)) {
						int out0 = 1;
						int out1 = 0;

						outputSignals[0]->bufferPut((t_binary)out0);
						outputSignals[0]->bufferPut((t_binary)out1);

					}
					else {
						inputSignals[0]->bufferGet(&a3);
						if ((a1 == 1) && (a2 == 1) && (a3 == 0)) {
							int out0 = 0;
							int out1 = 1;

							outputSignals[0]->bufferPut((t_binary)out0);
							outputSignals[0]->bufferPut((t_binary)out1);

						}
						else {
							int out0 = 1;
							int out1 = 1;

							outputSignals[0]->bufferPut((t_binary)out0);
							outputSignals[0]->bufferPut((t_binary)out1);
						}
					}

				}
			}
			else {
				if ((a1 == 0)) {
					int out0 = 1;
					int out1 = 1;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);

				}
				else {
					inputSignals[0]->bufferGet(&a2);
					if ((a1 == 1) && (a2 == 0)) {
						int out0 = 0;
						int out1 = 1;

						outputSignals[0]->bufferPut((t_binary)out0);
						outputSignals[0]->bufferPut((t_binary)out1);

					}
					else {
						inputSignals[0]->bufferGet(&a3);
						if ((a1 == 1) && (a2 == 1) && (a3 == 0)) {
							int out0 = 1;
							int out1 = 0;

							outputSignals[0]->bufferPut((t_binary)out0);
							outputSignals[0]->bufferPut((t_binary)out1);

						}
						else {
							int out0 = 0;
							int out1 = 0;

							outputSignals[0]->bufferPut((t_binary)out0);
							outputSignals[0]->bufferPut((t_binary)out1);
						}
					}

				}
			}

			break;
		case 3:
			inputSignals[0]->bufferGet(&a1);
			if (probabilityOfZero > 0.5) {
				if ((a1 == 0)) {
					int out0 = 0;
					int out1 = 0;
					int out2 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);

				}
				else {
					inputSignals[0]->bufferGet(&a2);
					if ((a1 == 1) && (a2 == 0)) {
						int out0 = 1;
						int out1 = 0;
						int out2 = 0;

						outputSignals[0]->bufferPut((t_binary)out0);
						outputSignals[0]->bufferPut((t_binary)out1);
						outputSignals[0]->bufferPut((t_binary)out2);

					}
					else {
						inputSignals[0]->bufferGet(&a3);
						if ((a1 == 1) && (a2 == 1) && (a3 == 0)) {
							int out0 = 0;
							int out1 = 1;
							int out2 = 0;

							outputSignals[0]->bufferPut((t_binary)out0);
							outputSignals[0]->bufferPut((t_binary)out1);
							outputSignals[0]->bufferPut((t_binary)out2);

						}
						else {
							inputSignals[0]->bufferGet(&a4);
							if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 0)) {
								int out0 = 1;
								int out1 = 1;
								int out2 = 0;

								outputSignals[0]->bufferPut((t_binary)out0);
								outputSignals[0]->bufferPut((t_binary)out1);
								outputSignals[0]->bufferPut((t_binary)out2);

							}
							else {
								inputSignals[0]->bufferGet(&a5);
								if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 0)) {
									int out0 = 0;
									int out1 = 0;
									int out2 = 1;

									outputSignals[0]->bufferPut((t_binary)out0);
									outputSignals[0]->bufferPut((t_binary)out1);
									outputSignals[0]->bufferPut((t_binary)out2);

								}
								else {
									inputSignals[0]->bufferGet(&a6);
									if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 0)) {
										int out0 = 1;
										int out1 = 0;
										int out2 = 1;

										outputSignals[0]->bufferPut((t_binary)out0);
										outputSignals[0]->bufferPut((t_binary)out1);
										outputSignals[0]->bufferPut((t_binary)out2);

									}
									else {
										inputSignals[0]->bufferGet(&a7);
										if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 0)) {
											int out0 = 0;
											int out1 = 1;
											int out2 = 1;

											outputSignals[0]->bufferPut((t_binary)out0);
											outputSignals[0]->bufferPut((t_binary)out1);
											outputSignals[0]->bufferPut((t_binary)out2);

										}
										else {
											int out0 = 1;
											int out1 = 1;
											int out2 = 1;

											outputSignals[0]->bufferPut((t_binary)out0);
											outputSignals[0]->bufferPut((t_binary)out1);
											outputSignals[0]->bufferPut((t_binary)out2);
										}
									}
								}
							}

						}

					}

				}
			}
			else {
				if ((a1 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);

				}
				else {
					inputSignals[0]->bufferGet(&a2);
					if ((a1 == 1) && (a2 == 0)) {
						int out0 = 0;
						int out1 = 1;
						int out2 = 1;

						outputSignals[0]->bufferPut((t_binary)out0);
						outputSignals[0]->bufferPut((t_binary)out1);
						outputSignals[0]->bufferPut((t_binary)out2);

					}
					else {
						inputSignals[0]->bufferGet(&a3);
						if ((a1 == 1) && (a2 == 1) && (a3 == 0)) {
							int out0 = 1;
							int out1 = 0;
							int out2 = 1;

							outputSignals[0]->bufferPut((t_binary)out0);
							outputSignals[0]->bufferPut((t_binary)out1);
							outputSignals[0]->bufferPut((t_binary)out2);

						}
						else {
							inputSignals[0]->bufferGet(&a4);
							if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 0)) {
								int out0 = 0;
								int out1 = 0;
								int out2 = 1;

								outputSignals[0]->bufferPut((t_binary)out0);
								outputSignals[0]->bufferPut((t_binary)out1);
								outputSignals[0]->bufferPut((t_binary)out2);

							}
							else {
								inputSignals[0]->bufferGet(&a5);
								if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 0)) {
									int out0 = 1;
									int out1 = 1;
									int out2 = 0;

									outputSignals[0]->bufferPut((t_binary)out0);
									outputSignals[0]->bufferPut((t_binary)out1);
									outputSignals[0]->bufferPut((t_binary)out2);

								}
								else {
									inputSignals[0]->bufferGet(&a6);
									if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 0)) {
										int out0 = 0;
										int out1 = 1;
										int out2 = 0;

										outputSignals[0]->bufferPut((t_binary)out0);
										outputSignals[0]->bufferPut((t_binary)out1);
										outputSignals[0]->bufferPut((t_binary)out2);

									}
									else {
										inputSignals[0]->bufferGet(&a7);
										if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 0)) {
											int out0 = 1;
											int out1 = 0;
											int out2 = 0;

											outputSignals[0]->bufferPut((t_binary)out0);
											outputSignals[0]->bufferPut((t_binary)out1);
											outputSignals[0]->bufferPut((t_binary)out2);

										}
										else {
											int out0 = 0;
											int out1 = 0;
											int out2 = 0;

											outputSignals[0]->bufferPut((t_binary)out0);
											outputSignals[0]->bufferPut((t_binary)out1);
											outputSignals[0]->bufferPut((t_binary)out2);

										}
									}
								}
							}

						}

					}

				}
			}
			break;
		case 4:
			inputSignals[0]->bufferGet(&a1);
			if (probabilityOfZero > 0.5) {
				if ((a1 == 0)) {
					int out0 = 0;
					int out1 = 0;
					int out2 = 0;
					int out3 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);


				}
				else {
					inputSignals[0]->bufferGet(&a2);
					if ((a1 == 1) && (a2 == 0)) {
						int out0 = 1;
						int out1 = 0;
						int out2 = 0;
						int out3 = 0;

						outputSignals[0]->bufferPut((t_binary)out0);
						outputSignals[0]->bufferPut((t_binary)out1);
						outputSignals[0]->bufferPut((t_binary)out2);
						outputSignals[0]->bufferPut((t_binary)out3);

					}
					else {
						inputSignals[0]->bufferGet(&a3);
						if ((a1 == 1) && (a2 == 1) && (a3 == 0)) {
							int out0 = 0;
							int out1 = 1;
							int out2 = 0;
							int out3 = 0;

							outputSignals[0]->bufferPut((t_binary)out0);
							outputSignals[0]->bufferPut((t_binary)out1);
							outputSignals[0]->bufferPut((t_binary)out2);
							outputSignals[0]->bufferPut((t_binary)out3);

						}
						else {
							inputSignals[0]->bufferGet(&a4);
							if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 0)) {
								int out0 = 1;
								int out1 = 1;
								int out2 = 0;
								int out3 = 0;

								outputSignals[0]->bufferPut((t_binary)out0);
								outputSignals[0]->bufferPut((t_binary)out1);
								outputSignals[0]->bufferPut((t_binary)out2);
								outputSignals[0]->bufferPut((t_binary)out3);

							}
							else {
								inputSignals[0]->bufferGet(&a5);
								if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 0)) {
									int out0 = 0;
									int out1 = 0;
									int out2 = 1;
									int out3 = 0;

									outputSignals[0]->bufferPut((t_binary)out0);
									outputSignals[0]->bufferPut((t_binary)out1);
									outputSignals[0]->bufferPut((t_binary)out2);
									outputSignals[0]->bufferPut((t_binary)out3);

								}
								else {
									inputSignals[0]->bufferGet(&a6);
									if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 0)) {
										int out0 = 1;
										int out1 = 0;
										int out2 = 1;
										int out3 = 0;

										outputSignals[0]->bufferPut((t_binary)out0);
										outputSignals[0]->bufferPut((t_binary)out1);
										outputSignals[0]->bufferPut((t_binary)out2);
										outputSignals[0]->bufferPut((t_binary)out3);

									}
									else {
										inputSignals[0]->bufferGet(&a7);
										if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 0)) {
											int out0 = 0;
											int out1 = 1;
											int out2 = 1;
											int out3 = 0;

											outputSignals[0]->bufferPut((t_binary)out0);
											outputSignals[0]->bufferPut((t_binary)out1);
											outputSignals[0]->bufferPut((t_binary)out2);
											outputSignals[0]->bufferPut((t_binary)out3);

										}
										else {
											inputSignals[0]->bufferGet(&a8);
											if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 0)) {
												int out0 = 1;
												int out1 = 1;
												int out2 = 1;
												int out3 = 0;

												outputSignals[0]->bufferPut((t_binary)out0);
												outputSignals[0]->bufferPut((t_binary)out1);
												outputSignals[0]->bufferPut((t_binary)out2);
												outputSignals[0]->bufferPut((t_binary)out3);

											}
											else {
												inputSignals[0]->bufferGet(&a9);
												if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 0)) {
													int out0 = 0;
													int out1 = 0;
													int out2 = 0;
													int out3 = 1;

													outputSignals[0]->bufferPut((t_binary)out0);
													outputSignals[0]->bufferPut((t_binary)out1);
													outputSignals[0]->bufferPut((t_binary)out2);
													outputSignals[0]->bufferPut((t_binary)out3);

												}
												else {
													inputSignals[0]->bufferGet(&a10);
													if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 0)) {
														int out0 = 1;
														int out1 = 0;
														int out2 = 0;
														int out3 = 1;

														outputSignals[0]->bufferPut((t_binary)out0);
														outputSignals[0]->bufferPut((t_binary)out1);
														outputSignals[0]->bufferPut((t_binary)out2);
														outputSignals[0]->bufferPut((t_binary)out3);

													}
													else {
														inputSignals[0]->bufferGet(&a11);
														if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 1) && (a11 == 0)) {
															int out0 = 0;
															int out1 = 1;
															int out2 = 0;
															int out3 = 1;

															outputSignals[0]->bufferPut((t_binary)out0);
															outputSignals[0]->bufferPut((t_binary)out1);
															outputSignals[0]->bufferPut((t_binary)out2);
															outputSignals[0]->bufferPut((t_binary)out3);

														}
														else {
															inputSignals[0]->bufferGet(&a12);
															if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 1) && (a11 == 1) && (a12 == 0)) {
																int out0 = 1;
																int out1 = 1;
																int out2 = 0;
																int out3 = 1;

																outputSignals[0]->bufferPut((t_binary)out0);
																outputSignals[0]->bufferPut((t_binary)out1);
																outputSignals[0]->bufferPut((t_binary)out2);
																outputSignals[0]->bufferPut((t_binary)out3);

															}
															else {
																inputSignals[0]->bufferGet(&a13);
																if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 1) && (a11 == 1) && (a12 == 1) && (a13 == 0)) {
																	int out0 = 0;
																	int out1 = 0;
																	int out2 = 1;
																	int out3 = 1;

																	outputSignals[0]->bufferPut((t_binary)out0);
																	outputSignals[0]->bufferPut((t_binary)out1);
																	outputSignals[0]->bufferPut((t_binary)out2);
																	outputSignals[0]->bufferPut((t_binary)out3);

																}
																else {
																	inputSignals[0]->bufferGet(&a14);
																	if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 1) && (a11 == 1) && (a12 == 1) && (a13 == 1) && (a14 == 0)) {
																		int out0 = 1;
																		int out1 = 0;
																		int out2 = 1;
																		int out3 = 1;

																		outputSignals[0]->bufferPut((t_binary)out0);
																		outputSignals[0]->bufferPut((t_binary)out1);
																		outputSignals[0]->bufferPut((t_binary)out2);
																		outputSignals[0]->bufferPut((t_binary)out3);

																	}
																	else {
																		inputSignals[0]->bufferGet(&a15);
																		if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 1) && (a11 == 1) && (a12 == 1) && (a13 == 1) && (a14 == 1) && (a15 == 0)) {
																			int out0 = 0;
																			int out1 = 1;
																			int out2 = 1;
																			int out3 = 1;

																			outputSignals[0]->bufferPut((t_binary)out0);
																			outputSignals[0]->bufferPut((t_binary)out1);
																			outputSignals[0]->bufferPut((t_binary)out2);
																			outputSignals[0]->bufferPut((t_binary)out3);

																		}

																		else {
																			int out0 = 1;
																			int out1 = 1;
																			int out2 = 1;
																			int out3 = 1;

																			outputSignals[0]->bufferPut((t_binary)out0);
																			outputSignals[0]->bufferPut((t_binary)out1);
																			outputSignals[0]->bufferPut((t_binary)out2);
																			outputSignals[0]->bufferPut((t_binary)out3);
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}

						}

					}

				}
			}
			else {
				if ((a1 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);


				}
				else {
					inputSignals[0]->bufferGet(&a2);
					if ((a1 == 1) && (a2 == 0)) {
						int out0 = 0;
						int out1 = 1;
						int out2 = 1;
						int out3 = 1;

						outputSignals[0]->bufferPut((t_binary)out0);
						outputSignals[0]->bufferPut((t_binary)out1);
						outputSignals[0]->bufferPut((t_binary)out2);
						outputSignals[0]->bufferPut((t_binary)out3);

					}
					else {
						inputSignals[0]->bufferGet(&a3);
						if ((a1 == 1) && (a2 == 1) && (a3 == 0)) {
							int out0 = 1;
							int out1 = 0;
							int out2 = 1;
							int out3 = 1;

							outputSignals[0]->bufferPut((t_binary)out0);
							outputSignals[0]->bufferPut((t_binary)out1);
							outputSignals[0]->bufferPut((t_binary)out2);
							outputSignals[0]->bufferPut((t_binary)out3);

						}
						else {
							inputSignals[0]->bufferGet(&a4);
							if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 0)) {
								int out0 = 0;
								int out1 = 0;
								int out2 = 1;
								int out3 = 1;

								outputSignals[0]->bufferPut((t_binary)out0);
								outputSignals[0]->bufferPut((t_binary)out1);
								outputSignals[0]->bufferPut((t_binary)out2);
								outputSignals[0]->bufferPut((t_binary)out3);

							}
							else {
								inputSignals[0]->bufferGet(&a5);
								if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 0)) {
									int out0 = 1;
									int out1 = 1;
									int out2 = 0;
									int out3 = 1;

									outputSignals[0]->bufferPut((t_binary)out0);
									outputSignals[0]->bufferPut((t_binary)out1);
									outputSignals[0]->bufferPut((t_binary)out2);
									outputSignals[0]->bufferPut((t_binary)out3);

								}
								else {
									inputSignals[0]->bufferGet(&a6);
									if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 0)) {
										int out0 = 0;
										int out1 = 1;
										int out2 = 0;
										int out3 = 1;

										outputSignals[0]->bufferPut((t_binary)out0);
										outputSignals[0]->bufferPut((t_binary)out1);
										outputSignals[0]->bufferPut((t_binary)out2);
										outputSignals[0]->bufferPut((t_binary)out3);

									}
									else {
										inputSignals[0]->bufferGet(&a7);
										if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 0)) {
											int out0 = 1;
											int out1 = 0;
											int out2 = 0;
											int out3 = 1;

											outputSignals[0]->bufferPut((t_binary)out0);
											outputSignals[0]->bufferPut((t_binary)out1);
											outputSignals[0]->bufferPut((t_binary)out2);
											outputSignals[0]->bufferPut((t_binary)out3);

										}
										else {
											inputSignals[0]->bufferGet(&a8);
											if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 0)) {
												int out0 = 0;
												int out1 = 0;
												int out2 = 0;
												int out3 = 1;

												outputSignals[0]->bufferPut((t_binary)out0);
												outputSignals[0]->bufferPut((t_binary)out1);
												outputSignals[0]->bufferPut((t_binary)out2);
												outputSignals[0]->bufferPut((t_binary)out3);

											}
											else {
												inputSignals[0]->bufferGet(&a9);
												if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 0)) {
													int out0 = 1;
													int out1 = 1;
													int out2 = 1;
													int out3 = 0;

													outputSignals[0]->bufferPut((t_binary)out0);
													outputSignals[0]->bufferPut((t_binary)out1);
													outputSignals[0]->bufferPut((t_binary)out2);
													outputSignals[0]->bufferPut((t_binary)out3);

												}
												else {
													inputSignals[0]->bufferGet(&a10);
													if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 0)) {
														int out0 = 0;
														int out1 = 1;
														int out2 = 1;
														int out3 = 0;

														outputSignals[0]->bufferPut((t_binary)out0);
														outputSignals[0]->bufferPut((t_binary)out1);
														outputSignals[0]->bufferPut((t_binary)out2);
														outputSignals[0]->bufferPut((t_binary)out3);

													}
													else {
														inputSignals[0]->bufferGet(&a11);
														if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 1) && (a11 == 0)) {
															int out0 = 1;
															int out1 = 0;
															int out2 = 1;
															int out3 = 0;

															outputSignals[0]->bufferPut((t_binary)out0);
															outputSignals[0]->bufferPut((t_binary)out1);
															outputSignals[0]->bufferPut((t_binary)out2);
															outputSignals[0]->bufferPut((t_binary)out3);

														}
														else {
															inputSignals[0]->bufferGet(&a12);
															if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 1) && (a11 == 1) && (a12 == 0)) {
																int out0 = 0;
																int out1 = 0;
																int out2 = 1;
																int out3 = 0;

																outputSignals[0]->bufferPut((t_binary)out0);
																outputSignals[0]->bufferPut((t_binary)out1);
																outputSignals[0]->bufferPut((t_binary)out2);
																outputSignals[0]->bufferPut((t_binary)out3);

															}
															else {
																inputSignals[0]->bufferGet(&a13);
																if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 1) && (a11 == 1) && (a12 == 1) && (a13 == 0)) {
																	int out0 = 1;
																	int out1 = 1;
																	int out2 = 0;
																	int out3 = 0;

																	outputSignals[0]->bufferPut((t_binary)out0);
																	outputSignals[0]->bufferPut((t_binary)out1);
																	outputSignals[0]->bufferPut((t_binary)out2);
																	outputSignals[0]->bufferPut((t_binary)out3);

																}
																else {
																	inputSignals[0]->bufferGet(&a14);
																	if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 1) && (a11 == 1) && (a12 == 1) && (a13 == 1) && (a14 == 0)) {
																		int out0 = 0;
																		int out1 = 1;
																		int out2 = 0;
																		int out3 = 0;

																		outputSignals[0]->bufferPut((t_binary)out0);
																		outputSignals[0]->bufferPut((t_binary)out1);
																		outputSignals[0]->bufferPut((t_binary)out2);
																		outputSignals[0]->bufferPut((t_binary)out3);

																	}
																	else {
																		inputSignals[0]->bufferGet(&a15);
																		if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1) && (a5 == 1) && (a6 == 1) && (a7 == 1) && (a8 == 1) && (a9 == 1) && (a10 == 1) && (a11 == 1) && (a12 == 1) && (a13 == 1) && (a14 == 1) && (a15 == 0)) {
																			int out0 = 1;
																			int out1 = 0;
																			int out2 = 0;
																			int out3 = 0;

																			outputSignals[0]->bufferPut((t_binary)out0);
																			outputSignals[0]->bufferPut((t_binary)out1);
																			outputSignals[0]->bufferPut((t_binary)out2);
																			outputSignals[0]->bufferPut((t_binary)out3);

																		}

																		else {
																			int out0 = 0;
																			int out1 = 0;
																			int out2 = 0;
																			int out3 = 0;

																			outputSignals[0]->bufferPut((t_binary)out0);
																			outputSignals[0]->bufferPut((t_binary)out1);
																			outputSignals[0]->bufferPut((t_binary)out2);
																			outputSignals[0]->bufferPut((t_binary)out3);
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}

						}

					}

				}
			}
			break;
		default:
			return alive;
			break;
		}
		*/

		ready = inputSignals[0]->ready();
		space = outputSignals[0]->space();
	}

	return alive;
}

void HuffmanDecoder::setProbabilityOfZero(double s_n) {
	probabilityOfZero = s_n;
}

double HuffmanDecoder::getProbabilityOfZero() {
	return probabilityOfZero;
}

void HuffmanDecoder::setSourceOrder(int s_n) {
	sourceOrder = s_n;
}

int HuffmanDecoder::getSourceOrder() {
	return sourceOrder;
}

bool HuffmanDecoder::getData(int counter) {
	int in;
	inputSignals[0]->bufferGet(&in);

	switch (sourceOrder) {
	case 2:
		if ((counter == 0) && (in == 0)) {
			int out0, out1;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 0;
			} else {
				out0 = 1;
				out1 = 1;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			return true;

		} else if ((counter == 1) && (in == 0)) {
			int out0, out1;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 0;
			} else {
				out0 = 0;
				out1 = 1;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			return true;

		} else if ((counter == 2) && (in == 0)) {
			int out0, out1;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 1;
			} else {
				out0 = 1;
				out1 = 0;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			return true;

		} else if ((counter == 2) && (in == 1)) {
			int out0, out1;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 1;
			} else {
				out0 = 0;
				out1 = 0;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			return true;
		}

		return false;
		break;

	case 3:
		if ((counter == 0) && (in == 0)) {
			int out0, out1, out2;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 0;
				out2 = 0;
			}
			else {
				out0 = 1;
				out1 = 1;
				out2 = 1;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);			
			outputSignals[0]->bufferPut((t_binary)out2);

			return true;

		}
		else if ((counter == 1) && (in == 0)) {
			int out0, out1, out2;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 0;
				out2 = 0;
			}
			else {
				out0 = 0;
				out1 = 1;
				out2 = 1;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);

			return true;

		}
		else if ((counter == 2) && (in == 0)) {
			int out0, out1, out2;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 1;
				out2 = 0;
			}
			else {
				out0 = 1;
				out1 = 0;
				out2 = 1;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);

			return true;

		}
		else if ((counter == 3) && (in == 0)) {
			int out0, out1, out2;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 1;
				out2 = 0;
			}
			else {
				out0 = 0;
				out1 = 0;
				out2 = 1;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);

			return true;
		} 
		else if ((counter == 4) && (in == 0)) {
			int out0, out1, out2;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 0;
				out2 = 1;
			}
			else {
				out0 = 1;
				out1 = 1;
				out2 = 0;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);

			return true;

		}
		else if ((counter == 5) && (in == 0)) {
			int out0, out1, out2;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 0;
				out2 = 1;
			}
			else {
				out0 = 0;
				out1 = 1;
				out2 = 0;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);

			return true;

		}
		else if ((counter == 6) && (in == 0)) {
			int out0, out1, out2;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 1;
				out2 = 1;
			}
			else {
				out0 = 1;
				out1 = 0;
				out2 = 0;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);

			return true;

		}
		else if ((counter == 6) && (in == 1)) {
			int out0, out1, out2;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 1;
				out2 = 1;
			}
			else {
				out0 = 0;
				out1 = 0;
				out2 = 0;
			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);

			return true;
		}

		return false;
		break;

	case 4:
		if ((counter == 0) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 0;
				out2 = 0;
				out3 = 0;
			}
			else {
				out0 = 1;
				out1 = 1;
				out2 = 1;
				out3 = 1;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;

		}
		else if ((counter == 1) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 0;
				out2 = 0;
				out3 = 0;
			}
			else {
				out0 = 0;
				out1 = 1;
				out2 = 1;
				out3 = 1;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);
			return true;

		}
		else if ((counter == 2) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 1;
				out2 = 0;
				out3 = 0;
			}
			else {
				out0 = 1;
				out1 = 0;
				out2 = 1;
				out3 = 1;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;

		}
		else if ((counter == 3) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 1;
				out2 = 0;
				out3 = 0;
			}
			else {
				out0 = 0;
				out1 = 0;
				out2 = 1;
				out3 = 1;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;
		}
		else if ((counter == 4) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 0;
				out2 = 1;
				out3 = 0;
			}
			else {
				out0 = 1;
				out1 = 1;
				out2 = 0;
				out3 = 1;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;

		}
		else if ((counter == 5) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 0;
				out2 = 1;
				out3 = 0;
			}
			else {
				out0 = 0;
				out1 = 1;
				out2 = 0;
				out3 = 1;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;

		}
		else if ((counter == 6) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 1;
				out2 = 1;
				out3 = 0;
			}
			else {
				out0 = 1;
				out1 = 0;
				out2 = 0;
				out3 = 1;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;

		}
		else if ((counter == 7) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 1;
				out2 = 1;
				out3 = 0;
			}
			else {
				out0 = 0;
				out1 = 0;
				out2 = 0;
				out3 = 1;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;
		}
		else if ((counter == 8) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 0;
				out2 = 0;
				out3 = 1;
			}
			else {
				out0 = 1;
				out1 = 1;
				out2 = 1;
				out3 = 0;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;
		}
		else if ((counter == 9) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 0;
				out2 = 0;
				out3 = 1;
			}
			else {
				out0 = 0;
				out1 = 1;
				out2 = 1;
				out3 = 0;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;
		}
		else if ((counter == 10) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 1;
				out2 = 0;
				out3 = 1;
			}
			else {
				out0 = 1;
				out1 = 0;
				out2 = 1;
				out3 = 0;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;
		}
		else if ((counter == 11) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 1;
				out2 = 0;
				out3 = 1;
			}
			else {
				out0 = 0;
				out1 = 0;
				out2 = 1;
				out3 = 0;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;
		}
		else if ((counter == 12) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 0;
				out2 = 1;
				out3 = 1;
			}
			else {
				out0 = 1;
				out1 = 1;
				out2 = 0;
				out3 = 0;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;
		}
		else if ((counter == 13) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 0;
				out2 = 1;
				out3 = 1;
			}
			else {
				out0 = 0;
				out1 = 1;
				out2 = 0;
				out3 = 0;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;
		}
		else if ((counter == 14) && (in == 0)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 0;
				out1 = 1;
				out2 = 1;
				out3 = 1;
			}
			else {
				out0 = 1;
				out1 = 0;
				out2 = 0;
				out3 = 0;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;
		}
		else if ((counter == 14) && (in == 1)) {
			int out0, out1, out2, out3;
			if (probabilityOfZero > 0.5) {
				out0 = 1;
				out1 = 1;
				out2 = 1;
				out3 = 1;
			}
			else {
				out0 = 0;
				out1 = 0;
				out2 = 0;
				out3 = 0;

			}

			outputSignals[0]->bufferPut((t_binary)out0);
			outputSignals[0]->bufferPut((t_binary)out1);
			outputSignals[0]->bufferPut((t_binary)out2);
			outputSignals[0]->bufferPut((t_binary)out3);

			return true;
		}

		return false;
		break;

	default:
		return false;
		break;
	}
}


