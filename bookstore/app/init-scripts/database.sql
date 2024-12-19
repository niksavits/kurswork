--
-- PostgreSQL database dump
--

-- Dumped from database version 16.6 (Ubuntu 16.6-0ubuntu0.24.04.1)
-- Dumped by pg_dump version 16.6 (Ubuntu 16.6-0ubuntu0.24.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: book_stores; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.book_stores (
    book_id integer NOT NULL,
    store_id integer NOT NULL
);


ALTER TABLE public.book_stores OWNER TO postgres;

--
-- Name: books; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.books (
    book_id integer NOT NULL,
    title character varying(255) NOT NULL,
    author character varying(255) NOT NULL,
    category character varying(100),
    publisher character varying(255),
    publisher_city character varying(100),
    publication_year integer,
    pages_count integer,
    price numeric(10,2),
    illustrations_count integer,
    store_id integer
);


ALTER TABLE public.books OWNER TO postgres;

--
-- Name: books_book_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.books_book_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.books_book_id_seq OWNER TO postgres;

--
-- Name: books_book_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.books_book_id_seq OWNED BY public.books.book_id;


--
-- Name: requests; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.requests (
    request_id integer NOT NULL,
    book_id integer NOT NULL,
    request_type character varying(50),
    title character varying(255),
    author character varying(255),
    category character varying(255),
    publisher character varying(255),
    publisher_city character varying(255),
    publication_year integer,
    pages_count integer,
    price numeric(10,2),
    illustrations_count integer,
    status character varying(50),
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP,
    store_name character varying(255),
    store_location character varying(255),
    working_hours character varying(255)
);


ALTER TABLE public.requests OWNER TO postgres;

--
-- Name: requests_request_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.requests_request_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.requests_request_id_seq OWNER TO postgres;

--
-- Name: requests_request_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.requests_request_id_seq OWNED BY public.requests.request_id;


--
-- Name: stores; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.stores (
    store_id integer NOT NULL,
    store_name character varying(255),
    store_location character varying(255),
    working_hours character varying(255)
);


ALTER TABLE public.stores OWNER TO postgres;

--
-- Name: stores_store_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.stores_store_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.stores_store_id_seq OWNER TO postgres;

--
-- Name: stores_store_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.stores_store_id_seq OWNED BY public.stores.store_id;


--
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    username character varying(255) NOT NULL,
    password character varying(255) NOT NULL,
    is_admin boolean DEFAULT false
);


ALTER TABLE public.users OWNER TO postgres;

--
-- Name: books book_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.books ALTER COLUMN book_id SET DEFAULT nextval('public.books_book_id_seq'::regclass);


--
-- Name: requests request_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.requests ALTER COLUMN request_id SET DEFAULT nextval('public.requests_request_id_seq'::regclass);


--
-- Name: stores store_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.stores ALTER COLUMN store_id SET DEFAULT nextval('public.stores_store_id_seq'::regclass);


--
-- Data for Name: book_stores; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.book_stores (book_id, store_id) FROM stdin;
\.


--
-- Data for Name: books; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.books (book_id, title, author, category, publisher, publisher_city, publication_year, pages_count, price, illustrations_count, store_id) FROM stdin;
1	Война и мир	Лев Толстой	Художественная литература	Эксмо	\N	1869	1225	500.00	100	\N
2	Гарри Поттер и философский камень	Дж.К. Роулинг	Детская литература	Росмэн	\N	1997	320	350.00	50	\N
3	Чистый код	Роберт Мартин	Учебная литература	Диалектика	\N	2008	464	1200.00	0	\N
4	Алхимик	Пауло Коэльо	Художественная литература	АСТ	\N	1988	208	400.00	10	\N
5	Основы программирования на C++	Бьярне Страуструп	Учебная литература	Питер	\N	2011	1024	1500.00	0	\N
6	Маленький принц	Антуан де Сент-Экзюпери	Детская литература	Махаон	\N	1943	96	250.00	30	\N
7	asd	asd	\N	asd	\N	\N	\N	125.00	\N	\N
8	asd	asd	sdf	asdf	asdf	1234	3264	324.00	52	\N
0	asd	asd	asdf	asdf	asdf	123	234	123.00	124	\N
77	as	as	as	as	as	77	77	77.00	77	4
\.


--
-- Data for Name: requests; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.requests (request_id, book_id, request_type, title, author, category, publisher, publisher_city, publication_year, pages_count, price, illustrations_count, status, created_at, store_name, store_location, working_hours) FROM stdin;
10	8	ADD_BOOK	asd	asd	sdf	asdf	asdf	1234	3264	324.00	52	approved	2024-12-16 14:19:20	\N	\N	\N
9	0	ADD_BOOK	asd	asd	asdf	asdf	asdf	123	234	123.00	124	approved	2024-12-16 14:17:03	\N	\N	\N
12	7	DELETE_BOOK	sdf	asdf	asdf	asdf	asdf	234	234	1234.00	123	reject	2024-12-16 16:10:30	\N	\N	\N
14	55	ADD_BOOK	asd	sadf	asdf	asdf	asdf	24	2345	34.00	34	approved	2024-12-16 18:47:26	dfgs	dsfgh	dsfhsh
17	55	ADD_BOOK	asdf	sadf	sdaf	asdf	asdf	2134	234	2134.00	23	reject	2024-12-16 20:04:38	asd	asdads	ads
18	55	DELETE_BOOK	as	sdf	sadf	sadf	sadf	24	21	321.00	231	approved	2024-12-16 20:06:01	\N	\N	\N
19	55	DELETE_BOOK	asdf	sadf	sad	sad	sad	22	2345	345.00	234	rejected	2024-12-16 20:09:19	\N	\N	\N
\.


--
-- Data for Name: stores; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.stores (store_id, store_name, store_location, working_hours) FROM stdin;
4	as	as	as
7	\N	\N	\N
\.


--
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.users (username, password, is_admin) FROM stdin;
postgres	password	t
new	new	f
zssad	sadf	f
sdfljk	\\x34626163323733393362646439373737636530323435333235366335353737636430323237353531306232323237663437336430336635333339323466383737	f
sdsdf	\\x34626163323733393362646439373737636530323435333235366335353737636430323237353531306232323237663437336430336635333339323466383737	f
sadhf	\\x36376336613663643261313632666232333337383837646663383530376335376663316133333737323638646537303339353635623161623439393333353165	f
shdlf	\\x33376336646631666539313839316363343934393837643034623830303936376661626530316431366161383832653730353532336362643830313636303134	f
jskg	\\x66656463316430396336646361346235643130653265666535623135613935333164336236393436373661336538306362386132333731383130346361656135	f
kaksdh	\\x65326663336164613033313766396439613637376331376463313565323532363965316238643339343463366139343234643061376166393461393135313134	f
sdjfhd	\\x37643637363338633335343132336231623235373337366434333634363337333932333631363866616235383564353130326161323366626238353762353561	f
asjldfg	\\x64383966663538666531323066326232343835366237663866306231366233616235616237623861323465346237376235653964323636363731613937656538	f
sadg	\\x63383162313861383361336562343462313534653938306138373339616439303664343735383534363839653938623833623733616661613634376337663630	f
hgsda	\\x61373038336666616235326464313535306531633161363333353966623933613335343534346231313761656138306237383965653766613534303735313961	f
lsjgadf	8283ea64f96fcb2900a44342b324af7464d6633b313a5331138c896a8b3dfc92	f
sadjlfh	a20fa16c0855e93ddf199d41522e019c60da18e71b40c09f7148face8830554a	f
ashgf	\\x118f95bd7231061ba0adf06bdedf22e2a28a3795aed00f6a8fe59b7361d2322a	f
asdkjf	76351620bd2ab04805de401d4ea864b77bf3efd86877bffad7ce6be902bbd436	f
lahsgd	c192c80a3cdaa2eb9c79b029089874b79be096732cc471ec5ab0d42df8a3d611	f
asdjlkf	43c089043ee8b8adb3d767aeaa36aaa7f5a26771d158a8060b5a3a9144862f9d	f
aslhdf	bbcfbb02df75d9460edd59d01f255a6b384ec682886cab4e3e5b7cbbacc64c97	f
sdfsaf	353bd02769676e2fe6b6ef86063066ca146a6d570c3bcefb3d199951483a332e	f
gsdf	5f269862ccdb52db7c7ad315a7376a4350d03a746ebe878971178132189126ca	f
user	04f8996da763b7a969b1028ee3007569eaf3a635486ddab211d512c85b9df8fb	f
hasgdfasdg	bbf78bc146f500fa4fea6306bebd5e0e8706a43a1df42aa977d8cba1434ef613	f
sdjfhalksdf	8743c8c0999a81fa37227c2872d1729b64db99d17d2cbfb1390ba9132838cb59	f
;ljshdfg	413777136e680856ad52d0f1e293d1e276fc1d09d6d18c5fae1784f13d1727d6	f
shladgf	6b9414d0ac09ec32ece7226b728bfed906d9ade7b6e0b453f6b39f90d5c180bb	f
hsad	1bad84b177c1da3bf22e5ab40015347abc1a26116db22dbbdf7dc03c657ed7b1	f
hjfdg	5c538087701ec207247ba60fd79dd11cf1bffcc571cf0e6e601281a7e0223ece	f
jhsfvgjsdf	21d8fdc480006cd35fadbb4b63cc248010dc9ffffc0183f0bc7259af1f13c195	f
slkjdfg	913617dd005f6c030eefb8c659edb4effae80446b86a31136e495bc32a73f2fc	f
zdshadlf	b323ba8eb5a4df2f162b251ea20b37f6887c7f88714de8ab70ebb4dc99beade5	f
newuser	9c9064c59f1ffa2e174ee754d2979be80dd30db552ec03e7e327e9b1a4bd594e	f
\.


--
-- Name: books_book_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.books_book_id_seq', 7, true);


--
-- Name: requests_request_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.requests_request_id_seq', 19, true);


--
-- Name: stores_store_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.stores_store_id_seq', 10, true);


--
-- Name: book_stores book_stores_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.book_stores
    ADD CONSTRAINT book_stores_pkey PRIMARY KEY (book_id, store_id);


--
-- Name: books books_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.books
    ADD CONSTRAINT books_pkey PRIMARY KEY (book_id);


--
-- Name: requests requests_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.requests
    ADD CONSTRAINT requests_pkey PRIMARY KEY (request_id);


--
-- Name: stores stores_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.stores
    ADD CONSTRAINT stores_pkey PRIMARY KEY (store_id);


--
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (username);


--
-- Name: book_stores book_stores_book_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.book_stores
    ADD CONSTRAINT book_stores_book_id_fkey FOREIGN KEY (book_id) REFERENCES public.books(book_id) ON DELETE CASCADE;


--
-- Name: book_stores book_stores_store_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.book_stores
    ADD CONSTRAINT book_stores_store_id_fkey FOREIGN KEY (store_id) REFERENCES public.stores(store_id) ON DELETE CASCADE;


--
-- Name: books fk_store_id; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.books
    ADD CONSTRAINT fk_store_id FOREIGN KEY (store_id) REFERENCES public.stores(store_id);


--
-- PostgreSQL database dump complete
--

